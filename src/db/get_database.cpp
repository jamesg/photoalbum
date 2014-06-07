/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "get_database.hpp"

#include <map>
#include <utility>

#include "sqlite/connection.hpp"

namespace
{
    static std::map<photograph::db::database_type, sqlite::connection> g_connections;
}

sqlite::connection& photograph::db::get_database(
        database_type db_type,
        const std::string& filename
        )
{
    if(g_connections.find(db_type) != g_connections.end())
        return g_connections.find(db_type)->second;

    g_connections.emplace(std::make_pair(db_type, sqlite::connection(filename)));
    return g_connections.find(db_type)->second;
}

