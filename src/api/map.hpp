#ifndef API_MAP_HPP
#define API_MAP_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/json.hpp"
#include "json/map_accessor.hpp"

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
        namespace map
        {
            /*
             * Struct for transferring base64-encoded PNG data.
             */
            struct png_data : public json::map_accessor
            {
                // Base64-encoded PNG data.
                std::string& data() const { return get_string("data"); }
                png_data(json::object& o) :
                    json::map_accessor(o)
                {
                }
            };

            void get_tile(
                const jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
            void get_tile_data(
                const jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
            void search_gazetteer(
                const jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
            void map_tile_km(
                const jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        }
    }
}

#endif

