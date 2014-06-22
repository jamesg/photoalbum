#ifndef PHOTOGRAPH_URI_MAP_TILE_KM_HPP
#define PHOTOGRAPH_URI_MAP_TILE_KM_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "mongoose.h"

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    class server;

    namespace uri
    {
        /*
         * Render a PNG image of a kilometre square map tile.
         */
        int map_tile_km(
                const server&,
                mg_connection*,
                mg_event,
                sqlite::connection& map_db,
                sqlite::connection& auth_db
                );
    }
}

#endif

