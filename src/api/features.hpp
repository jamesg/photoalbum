#ifndef PHOTOGRAPH_API_FEATURES_HPP
#define PHOTOGRAPH_API_FEATURES_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/map_accessor.hpp"

namespace jsonrpc
{
    class request;
    class result;
}

namespace photograph
{
    /*
     * Set of features supported by this server.
     */
    struct features : public json::map_accessor
    {
        bool& gazetteer() const { return get_bool("gazetteer"); }
        bool& photographs() const { return get_bool("photographs"); }

        features(json::object& o) : json::map_accessor(o)
        {
        }
    };

    /*
     * Get the global features object.
     */
    const features get_features();

    namespace api
    {
        void features(
                const jsonrpc::request&,
                jsonrpc::result&
                );
    }
}

#endif

