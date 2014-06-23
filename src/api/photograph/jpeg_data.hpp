#ifndef API_JPEG_DATA_HPP
#define API_JPEG_DATA_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace jsonrpc
{
    struct request;
    struct result;
}

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace api
    {
        void insert_jpeg_data(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void jpeg_data(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void jpeg_data_scaled(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
    }
}

#endif

