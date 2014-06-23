#ifndef PHOTOALBUM_API_TAGS_HPP
#define PHOTOALBUM_API_TAGS_HPP

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
        void tags(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void tags_alphabetical(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void tags_popular(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
    }
}

#endif

