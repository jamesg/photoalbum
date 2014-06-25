#ifndef JSONRPC_RESULT_HPP
#define JSONRPC_RESULT_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/json.hpp"
#include "json/map_accessor.hpp"

namespace jsonrpc
{
    /*!
     * Accessor struct for the result of a JSONRPC result.
     */
    struct result : public json::map_accessor
    {
        result( json::object& m ) :
            map_accessor( m )
        {
        }

        std::string& error() { return get_string("error"); }
        json::object& data() { return get_object("result"); }
    };
}

#endif

