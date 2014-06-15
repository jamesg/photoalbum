#ifndef JSONRPC_REQUEST_HPP
#define JSONRPC_REQUEST_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/json.hpp"
#include "json/map_accessor.hpp"

namespace jsonrpc
{
    /*
     * Accessor for a JSONRPC request.
     */
    struct request : public json::map_accessor
    {
        request( json::object& m ) :
            map_accessor( m )
        {
        }

        std::string& method() const { return get_string("method"); }
        json::list& params() const { return get_list("params"); }
        std::string& token() const { return get_string("token"); }
    };
}

#endif

