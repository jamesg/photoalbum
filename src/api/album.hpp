#ifndef API_ALBUM_HPP
#define API_ALBUM_HPP

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

namespace photograph
{
    namespace api
    {
        void add_photograph_to_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void album_list(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void insert_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void uncategorised_photographs(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void update_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void delete_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
    }
}

#endif

