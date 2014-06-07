/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jpeg_image_fullsize.hpp"

#include <boost/lexical_cast.hpp>

#include "db/get_database.hpp"
#include "db/jpeg_data.hpp"
#include "server.hpp"
#include "uri/http_date.hpp"

int photograph::uri::jpeg_image_fullsize(
        const server& serve,
        mg_connection *conn,
        mg_event ev
        )
{
    if(ev != MG_REQUEST)
        return MG_FALSE;
    mg_send_status(conn, 200);
    mg_send_header(conn, "Content-type", "image/jpeg");
    // TODO write this comment
    mg_send_header(conn, "Last-Modified", http_date(serve.start_time()).c_str());

    char field[100];
    mg_get_var(conn, "photograph_id", field, sizeof(field));

    try
    {
        int photo_id = boost::lexical_cast<int>(field);
        json::object data_o;
        jpeg_data_db data(data_o);
        db::get_by_id(
                photo_id,
                db::get_database(db::database_photograph),
                data
                );
        mg_send_data(conn, &(data.data[0]), data.data.size());
    }
    catch(const std::exception& e)
    {
        std::cerr << "in jpeg_image_fullsize: " << e.what() << std::endl;
        return MG_FALSE;
    }
    return MG_TRUE;
}

