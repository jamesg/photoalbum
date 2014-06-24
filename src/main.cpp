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

#include "api/api.hpp"
#include "db/auth.hpp"
#include "db/cache.hpp"
#include "db/map.hpp"
#include "db/photograph.hpp"
#include "jsonrpc/auth.hpp"
#include "jsonrpc/server.hpp"
#include "server.hpp"
#include "uri/insert_photograph.hpp"
#include "uri/jpeg_image.hpp"
#include "uri/jpeg_image_fullsize.hpp"
#include "uri/map_tile_km.hpp"

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
        pem_file,
        port = "4008",
        auth_db_file;

    std::vector<commandline::option> options{
        commandline::flag("help", show_help, "Print a help message"),
        commandline::parameter("db", db_file, "Path to the photograph database"),
        commandline::parameter("mapdb", map_db_file, "Path to a read-only map database"),
        commandline::parameter("auth_db", auth_db_file, "Path to the authentication database"),
        commandline::parameter("document-root", document_root, "HTML document root"),
        commandline::parameter("pem", pem_file, "PEM file"),
        commandline::parameter("port", port, "Port number to bind on")
    };

    commandline::parse(argc, argv, options);

    if(show_help)
    {
        commandline::print(argc, argv, options);
        return 0;
    }

    photoalbum::features f = photoalbum::get_features();

    if(db_file.size())
        f.photographs() = true;

    if(map_db_file.size())
        f.gazetteer() = true;

    boost::asio::ip::address bind_addr;

    sqlite::connection db =
        db_file.length()?
        sqlite::connection(db_file):
        sqlite::connection::in_memory_database();
    sqlite::connection map_db =
        map_db_file.length()?
        sqlite::connection(map_db_file):
        sqlite::connection::in_memory_database();
    photoalbum::db::map::create(map_db);

    sqlite::connection auth_db =
        auth_db_file.length()?
        sqlite::connection(auth_db_file):
        sqlite::connection::in_memory_database();
    photoalbum::db::auth::create(auth_db);

    sqlite::connection cache_db = sqlite::connection::in_memory_database();
    photoalbum::db::cache::create(cache_db);

    jsonrpc::server api_server;

    photoalbum::api::photograph::install(db, auth_db, api_server);
    photoalbum::api::map::install(map_db, auth_db, api_server);
    photoalbum::api::auth::install(auth_db, api_server);

    api_server.install(
        "features",
        boost::bind(photoalbum::api::features, _1, _2)
        );
    api_server.install(
        "statistics",
        boost::bind(photoalbum::api::statistics, _1, _2, boost::ref(db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );

    photoalbum::server s(document_root, port, pem_file);

    s.install(
            "/jpeg_image",
            boost::bind(&photoalbum::uri::jpeg_image, boost::ref(s), _1, _2, boost::ref(db), boost::ref(auth_db), boost::ref(cache_db))
            );
    s.install(
            "/jpeg_image_fullsize",
            boost::bind(&photoalbum::uri::jpeg_image_fullsize, boost::ref(s), _1, _2, boost::ref(db), boost::ref(auth_db))
            );
    s.install(
            "/map_tile_km",
            boost::bind(&photoalbum::uri::map_tile_km, boost::ref(s), _1, _2, boost::ref(map_db), boost::ref(auth_db))
            );
    s.install(
            "/insert_photograph",
            boost::bind(&photoalbum::uri::insert_photograph, _1, _2, boost::ref(db))
            );
    s.install("/api_call", boost::bind(&api_call, api_server, _1, _2));
    s.listen();
}

