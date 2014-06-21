#ifndef PHOTOGRAPH_DB_AUTH_HPP
#define PHOTOGRAPH_DB_AUTH_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

#include "db/has_id.hpp"

namespace sqlite
{
    class connection;
}

namespace photograph
{
    namespace auth
    {
        extern const char user_id_cstr[];

        struct user : public has_id<user_id_cstr>
        {
            user(json::object& o) :
                has_id(o)
            {
            }
            std::string& username() const { return get_string("username"); }
            std::string& password() const { return get_string("password"); }
        };

        struct user_details : public has_id<user_id_cstr>
        {
            user_details(json::object& o) :
                has_id(o)
            {
            }
            std::string& first_name() const { return get_string("first_name"); }
            std::string& last_name() const { return get_string("last_name"); }
        };
    }

    namespace db
    {
        namespace auth
        {
            /*
             * The length of an authentication token when represented as a
             * string.  Does not include the null terminator.
             */
            static const int token_length = 64;

            /*
             * Populate an authentication database with the required tables.
             *
             * Tables are created with 'IF NOT EXISTS', so this function can be
             * called safely on a database that has already been populated.
             */
            void create(sqlite::connection& auth_db);

            /*
             * Check if a token is valid at the present time.
             *
             * Tokens are considered valid if their creation date is in the
             * past, their expiry date is in the future and they have not been
             * explicitly invalidated.
             */
            bool token_valid(const std::string& token, sqlite::connection&);

            /*
             * Issue a token.
             */
            void issue_token(
                    const std::string& token,
                    const int          user_id,
                    sqlite::connection&
                    );

            /*
             * Invalidate a token.
             */
            void invalidate(const std::string& token, sqlite::connection&);

            /*
             * Get the user associated with a token.
             */
            void token_user(
                    sqlite::connection&     auth_db,
                    const std::string&      token,
                    photograph::auth::user& user
                    );

            /*
             * Get the user by their username.
             */
            void username_user(
                    sqlite::connection&     auth_db,
                    const std::string&      username,
                    photograph::auth::user& user
                    );

            /*
             * Delete tokens that have expired because the expiry time has
             * passed.
             */
            void delete_old_tokens(sqlite::connection& auth_db);
        }

        void get_by_id(
                int user_id,
                sqlite::connection& auth_db,
                const photograph::auth::user& user
                );

        void update(
                const photograph::auth::user& user,
                sqlite::connection& auth_db
                );
    }
}

#endif

