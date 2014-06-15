/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jsonrpc/auth.hpp"

#include "db/auth.hpp"
#include "jsonrpc/request.hpp"

bool jsonrpc::auth::logged_in(
        sqlite::connection& conn,
        const request& req
        )
{
    return photograph::db::auth::token_valid(req.token(), conn);
}

