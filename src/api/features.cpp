/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "features.hpp"

#include <boost/scoped_ptr.hpp>

#include "json/json.hpp"

#include "jsonrpc/result.hpp"

namespace
{
    static boost::scoped_ptr<json::object> g_features;
}

const photoalbum::features photoalbum::get_features()
{
    if(!g_features)
        g_features.reset(new json::object);

    return features(*g_features);
}

void photoalbum::api::features(
        const jsonrpc::request&,
        jsonrpc::result& result
        )
{
    result.data() = get_features().get_object();
}

