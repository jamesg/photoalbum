/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <cassert>
#include <locale>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/tokenizer.hpp>

#include "mongoose.h"

#include "commandline/commandline.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/bind_values.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"

#include "db/auth.hpp"
#include "db/get_database.hpp"
#include "db/jpeg_data.hpp"
#include "db/photograph.hpp"
#include "jsonrpc/auth.hpp"
#include "jsonrpc/server.hpp"
#include "server.hpp"
#include "uri/insert_photograph.hpp"
#include "uri/jpeg_image.hpp"
#include "uri/jpeg_image_fullsize.hpp"
#include "photograph_api.hpp"
#include "photograph_db.hpp"

//temp
#include <exiv2/exiv2.hpp>
#include <Magick++.h>
#include <sqlite3.h>
#include "sqlite/bind_values.hpp"
#include "sqlite/update.hpp"
#include "base64/base64.hpp"

namespace
{
    int api_call(const jsonrpc::server& server, mg_connection *conn, mg_event ev)
    {
        if(ev != MG_REQUEST)
            return MG_FALSE;
        json::object req_o;
        json::parse_json(std::string(conn->content), req_o);
        jsonrpc::request req(req_o);

        json::object res_o;
        jsonrpc::result res(res_o);

        server.serve(req, res);

        mg_send_status(conn, 200);
        mg_send_header(conn, "Content-type", "text/json");
        std::string json_s = json::serialise_json(res_o);
        mg_send_data(conn, json_s.c_str(), json_s.length());
        return MG_TRUE;
    }
}

int main(int argc, const char* argv[])
{
    bool show_help = false;
    std::string
        db_file,
        map_db_file,
        document_root,
        port = "4008",
        auth_db_file;

    std::vector<commandline::option> options{
        commandline::flag("help", show_help, "Print a help message"),
        commandline::parameter("db", db_file, "Path to the photograph database"),
        commandline::parameter("mapdb", map_db_file, "Path to a read-only map database"),
        commandline::parameter("auth_db", auth_db_file, "Path to the authentication database"),
        commandline::parameter("document-root", document_root, "HTML document root"),
        commandline::parameter("port", port, "Port number to bind on")
    };

    commandline::parse(argc, argv, options);

    if(show_help)
    {
        commandline::print(argc, argv, options);
        return 0;
    }

    photograph::features f = photograph::get_features();

    if(db_file.size())
        f.photographs() = true;

    if(map_db_file.size())
        f.gazetteer() = true;

    boost::asio::ip::address bind_addr;

    sqlite::connection& db = photograph::db::get_database(photograph::db::database_photograph, db_file);
    sqlite::connection& map_db = photograph::db::get_database(photograph::db::database_map, map_db_file);
    sqlite::connection auth_db = 
        auth_db_file.length()?
        sqlite::connection(auth_db_file):
        sqlite::connection::in_memory_database();
    photograph::db::auth::create(auth_db);
    sqlite::connection cache_db = sqlite::connection::in_memory_database();
    sqlite::devoid(
            "CREATE TABLE jpeg_cache ( "
            " photograph_id INTEGER, "
            " width INTEGER, "
            " height INTEGER, "
            " data BLOB, "
            " PRIMARY KEY(photograph_id, width, height) "
            " ) ",
            boost::fusion::vector<>(),
            cache_db
            );

    jsonrpc::server api_server;

    // Features.
    api_server.install(
            "features",
            boost::bind(photograph::api::features, _1, _2)
            );

    // Album methods.
    api_server.install(
        "add_photograph_to_album",
        boost::bind(photograph::api::add_photograph_to_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "album_list",
        boost::bind(photograph::api::album_list, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "delete_album",
        boost::bind(photograph::api::delete_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "insert_album",
        boost::bind(photograph::api::insert_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "update_album",
        boost::bind(photograph::api::update_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // JPEG data methods.
    api_server.install(
        "insert_jpeg_data",
        boost::bind(photograph::api::insert_jpeg_data, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "jpeg_data",
        boost::bind(photograph::api::jpeg_data, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "jpeg_data_scaled",
        boost::bind(photograph::api::jpeg_data_scaled, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    api_server.install(
        "locations",
        boost::bind(photograph::api::locations, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "tags",
        boost::bind(photograph::api::tags, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // Photograph methods.
    api_server.install(
        "photograph",
        boost::bind(photograph::api::photograph, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "delete_photograph",
        boost::bind(photograph::api::delete_photograph, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "insert_photograph",
        boost::bind(photograph::api::insert_photograph, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "photograph_albums",
        boost::bind(photograph::api::photograph_albums, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "photograph_list",
        boost::bind(photograph::api::photograph_list, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "photographs_in_album",
        boost::bind(photograph::api::photographs_in_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "photographs_with_location",
        boost::bind(photograph::api::photographs_with_location, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "photographs_with_tag",
        boost::bind(photograph::api::photographs_with_tag, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "recent_photographs",
        boost::bind(photograph::api::recent_photographs, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "remove_photograph_from_album",
        boost::bind(photograph::api::remove_photograph_from_album, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "uncategorised_photographs",
        boost::bind(photograph::api::uncategorised_photographs, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "update_photograph",
        boost::bind(photograph::api::update_photograph, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // Note methods.
    api_server.install(
        "markdown_data",
        boost::bind(photograph::api::markdown_data, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "note",
        boost::bind(photograph::api::note, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "note_list",
        boost::bind(photograph::api::note_list, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "note_version",
        boost::bind(photograph::api::note_version, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "note_phase_version",
        boost::bind(photograph::api::note_phase_version, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
            "create_draft_note",
            boost::bind(photograph::api::create_draft_note, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
            );
    api_server.install(
        "delete_note",
        boost::bind(photograph::api::delete_note, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "update_markdown_data",
        boost::bind(photograph::api::update_markdown_data, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "publish_note",
        boost::bind(photograph::api::publish_note, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "published_notes",
        boost::bind(photograph::api::published_notes, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // Statistics methods.
    api_server.install(
        "statistics",
        boost::bind(photograph::api::statistics, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // Map methods.
    api_server.install(
        "get_tile",
        boost::bind(photograph::api::map::get_tile, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "get_tile_data",
        boost::bind(photograph::api::map::get_tile_data, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "search_gazetteer",
        boost::bind(photograph::api::map::search_gazetteer, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "map_tile_km",
        boost::bind(photograph::api::map::map_tile_km, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    // Authentication functions.
    api_server.install(
        "login",
        boost::bind(photograph::api::auth::login, _1, _2, boost::ref(auth_db))
        );
    api_server.install(
        "logout",
        boost::bind(photograph::api::auth::logout, _1, _2, boost::ref(auth_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "token_valid",
        boost::bind(photograph::api::auth::token_valid, _1, _2, boost::ref(auth_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    photograph::server s(document_root, port);

    s.install(
            "/jpeg_image",
            boost::bind(&photograph::uri::jpeg_image, boost::ref(s), _1, _2, boost::ref(cache_db))
            );
    s.install(
            "/jpeg_image_fullsize",
            boost::bind(&photograph::uri::jpeg_image_fullsize, boost::ref(s), _1, _2)
            );
    s.install("/insert_photograph", &photograph::uri::insert_photograph);
    s.install("/api_call", boost::bind(&api_call, api_server, _1, _2));
    s.listen();
}

