#ifndef PHOTOGRAPH_API_AUTH_HPP
#define PHOTOGRAPH_API_AUTH_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

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
        namespace auth
        {
            /*
             * Generate a token if the username and password are correct.
             *
             * Should be called when the user logs in.  The generated token
             * should be sent with all future API requests.
             *
             * Parameters: [ username, password ].
             */
            void login(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
            /*
             * Immediately invalidate a token.
             *
             * Parameters: [ token ].
             *
             * Should be called when the user logs out.  The authentication
             * token will fail in all future requests, so the client should
             * forget the token after calling this function.
             */
            void logout(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
            /*
             * Check if an authentication token is valid.
             *
             * Parameters: [ token ].
             */
            void token_valid(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection& db
                );
        }
    }
}

#endif

