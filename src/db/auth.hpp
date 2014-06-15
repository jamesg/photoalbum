#ifndef PHOTOGRAPH_DB_AUTH_HPP
#define PHOTOGRAPH_DB_AUTH_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

namespace sqlite
{
    class connection;
}

namespace photograph
{
    namespace db
    {
        namespace auth
        {
            /*
             * Populate an authentication database with the required tables.
             *
             * Tables are created with 'IF NOT EXISTS', so this function can be
             * called safely on a database that has already been populated.
             */
            void create(sqlite::connection& auth_db);

            /*
             * Check if a token is valid at the present time.
             */
            bool token_valid(const std::string& token, sqlite::connection&);

            /*
             * Issue a token.
             */
            void issue_token(const std::string& token, sqlite::connection&);

            /*
             * Invalidate a token.
             */
            void invalidate(const std::string& token, sqlite::connection&);
        }
    }
}

#endif

