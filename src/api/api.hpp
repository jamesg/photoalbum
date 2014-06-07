#ifndef API_API_HPP
#define API_API_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jsonrpc/method_type.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/bind_values.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "photograph_db.hpp"

#include "sqlite/bind_values.hpp"
#include "sqlite/update.hpp"

#include "album.hpp"
#include "features.hpp"
#include "jpeg_data.hpp"
#include "map.hpp"
#include "note.hpp"
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

namespace photograph
{
    namespace api
    {
        void photograph(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
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

