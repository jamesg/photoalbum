/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api.hpp"

#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

#include "db/statistics.hpp"

void photoalbum::api::statistics(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    try
    {
        db::get(db, ::photoalbum::statistics(result.data()));
    }
    catch(const std::exception& e)
    {
        result.error() = "Database error";
        throw e;
    }
}

