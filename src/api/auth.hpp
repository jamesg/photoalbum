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

namespace photoalbum
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
            /*
             * Update a user account.
             *
             * This function should be used to update any details of the
             * auth_user, including the password.  The username cannot be
             * changed.  Users can only change their own account, with the
             * exception of the root user who can modify anyone's account.
             */
            void update_user(
                jsonrpc::request& request,
                jsonrpc::result& result,
                sqlite::connection& auth_db
                );
            /*
             * Get details of the currently logged in user.
             *
             * This function accepts no parameters, as the currently logged in
             * user is determined from the token.
             */
            void logged_in_user(
                jsonrpc::request& request,
                jsonrpc::result& result,
                sqlite::connection& auth_db
                );
        }
    }
}

#endif

