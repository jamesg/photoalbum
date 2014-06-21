/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "map_tile_km.hpp"

#include <boost/fusion/include/at_c.hpp>
#include <boost/lexical_cast.hpp>

#include "sqlite/connection.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

#include "db/auth.hpp"
#include "db/map.hpp"
#include "server.hpp"
#include "uri/detail.hpp"

int photograph::uri::map_tile_km(
        const server& serve,
        mg_connection *conn,
        mg_event ev,
        sqlite::connection& map_db,
        sqlite::connection& auth_db
        )
{
    if(ev != MG_REQUEST)
        return MG_FALSE;
    if(!db::auth::token_valid(detail::extract_token(conn), auth_db))
    {
        detail::text_response(conn, detail::status_unauthorized);
        return MG_TRUE;
    }
    char region_s[10], eastings_s[10], northings_s[10];
    mg_get_var(conn, "region", region_s, sizeof(region_s));
    mg_get_var(conn, "eastings", eastings_s, sizeof(eastings_s));
    mg_get_var(conn, "northings", northings_s, sizeof(northings_s));

    const int eastings = boost::lexical_cast<int>(eastings_s);
    const int northings = boost::lexical_cast<int>(northings_s);

    photograph::map::tile_data_db data_db;
    db::get(
            map_db,
            region_s,
            eastings,
            northings,
            data_db
            );
    mg_send_status(conn, 200);
    mg_send_header(conn, "Content-type", "image/png");
    mg_send_header(
            conn,
            "Last-Modified",
            detail::http_date(serve.start_time()).c_str()
            );
    mg_send_data(
            conn,
            &(data_db.data.front()),
            data_db.data.size()
            );
    return MG_TRUE;
}

