/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "map.hpp"

#include <boost/bind.hpp>

#include <Magick++.h>

#include "base64/base64.hpp"
#include "db/map.hpp"
#include "jsonrpc/auth.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"
#include "jsonrpc/server.hpp"

void photoalbum::api::map::install(
        sqlite::connection& map_db,
        sqlite::connection& auth_db,
        jsonrpc::server& api_server
        )
{
    api_server.install(
        "get_tile",
        boost::bind(photoalbum::api::map::get_tile, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "get_tile_data",
        boost::bind(photoalbum::api::map::get_tile_data, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "search_gazetteer",
        boost::bind(photoalbum::api::map::search_gazetteer, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
    api_server.install(
        "map_tile_km",
        boost::bind(photoalbum::api::map::map_tile_km, _1, _2, boost::ref(map_db)),
        boost::bind(jsonrpc::auth::logged_in, boost::ref(auth_db), _1)
        );
}

void photoalbum::api::map::get_tile(
        const jsonrpc::request& request,
        jsonrpc::result&        result,
        sqlite::connection&     conn
        )
{
    const std::string region = request.params().get<std::string>(0);
    const int eastings = request.params().get<int>(1);
    const int northings = request.params().get<int>(2);
    db::get(
            conn,
            region,
            eastings,
            northings,
            photoalbum::map::tile(result.data())
            );
}

void photoalbum::api::map::get_tile_data(
        const jsonrpc::request& request,
        jsonrpc::result&        result,
        sqlite::connection&     conn
        )
{
    const std::string region = request.params().get<std::string>(0);
    const int eastings = request.params().get<int>(1);
    const int northings = request.params().get<int>(2);
    db::get(
            conn,
            region,
            eastings,
            northings,
            photoalbum::map::tile_data(result.data())
            );
}

void photoalbum::api::map::search_gazetteer(
        const jsonrpc::request& request,
        jsonrpc::result&        result,
        sqlite::connection&     conn
        )
{
    json::list out;
    const std::string search_term = request.params().get<std::string>(0);
    db::search_gazetteer(
            conn,
            search_term,
            out
            );
    result.data() = out;
}

void photoalbum::api::map::map_tile_km(
        const jsonrpc::request& request,
        jsonrpc::result&        result,
        sqlite::connection&     conn
        )
{
    const std::string region = request.params().get<std::string>(0);
    const int eastings = request.params().get<int>(1);
    const int northings = request.params().get<int>(2);

    photoalbum::map::tile_data_db data_db;
    db::get(
            conn,
            region,
            eastings/10,
            northings/10,
            data_db
            );
    try
    {
        Magick::Image image(Magick::Blob(
            reinterpret_cast<const void*>(&(data_db.data[0])),
            data_db.data.size()
            )
            );
        Magick::Geometry out_size(
            image.size().width()/5,
            image.size().height()/5
            );
        Magick::Image out_image(out_size, Magick::Color(255,255,255));

        int off_x = -(out_size.width() * (eastings%5));
        int off_y = -(out_size.height() * (4 - (northings%5)));
        out_image.composite(image, off_x, off_y);

        Magick::Blob blob;
        out_image.write(&blob, "PNG");
        png_data(result.data()).data() = base64::encode(
            reinterpret_cast<const unsigned char*>(blob.data()),
            blob.length()
            );
    }
    catch(const std::exception& e)
    {
    }
}

