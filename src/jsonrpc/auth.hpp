#ifndef PHOTOGRAPH_JSONRPC_AUTH_HPP
#define PHOTOGRAPH_JSONRPC_AUTH_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace sqlite
{
    class connection;
}

namespace jsonrpc
{
    struct request;

    namespace auth
    {
        bool logged_in(sqlite::connection&, const request&);
    }
}

#endif

