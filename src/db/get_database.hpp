#ifndef PHOTOGRAPH_DB_GET_DATABASE
#define PHOTOGRAPH_DB_GET_DATABASE

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace db
    {
        enum database_type
        {
            database_photograph,
            database_map
        };

        sqlite::connection& get_database(database_type, const std::string& = "");
    }
}

#endif

