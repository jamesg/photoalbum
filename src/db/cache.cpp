/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "cache.hpp"

#include "sqlite/devoid.hpp"
#include "sqlite/row.hpp"

void photoalbum::db::cache::create(sqlite::connection& cache_db)
{
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS jpeg_cache ( "
            " photograph_id INTEGER, "
            " width INTEGER, "
            " height INTEGER, "
            " data BLOB, "
            " PRIMARY KEY(photograph_id, width, height) "
            " ) ",
            sqlite::empty_row(),
            cache_db
            );
}

