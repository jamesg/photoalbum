#ifndef API_API_HPP
#define API_API_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jsonrpc/method_type.hpp"

#include "sqlite/bind_values.hpp"
#include "sqlite/update.hpp"

#include "auth.hpp"
#include "features.hpp"
#include "map.hpp"
#include "note.hpp"
#include "photograph/album.hpp"
#include "photograph/jpeg_data.hpp"
#include "photograph/tags.hpp"
#include "photograph.hpp"
#include "statistics.hpp"

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
        void recent_photographs(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void locations(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        void tags(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
    }
}

#endif

