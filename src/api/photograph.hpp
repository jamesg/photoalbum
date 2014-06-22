#ifndef API_PHOTOGRAPH_HPP
#define API_PHOTOGRAPH_HPP

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
        void photograph(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void delete_photograph(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void insert_photograph(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void photograph_albums(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void photograph_list(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void photographs_in_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void remove_photograph_from_album(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void update_photograph(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void photographs_with_tag(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void photographs_with_location(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
    }
}

#endif

