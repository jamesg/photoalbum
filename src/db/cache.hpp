#ifndef PHOTOALBUM_DB_CACHE_HPP
#define PHOTOALBUM_DB_CACHE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace db
    {
        namespace cache
        {
            void create(sqlite::connection& cache_db);
        }
    }
}

#endif

