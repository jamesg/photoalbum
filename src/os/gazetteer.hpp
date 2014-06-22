#ifndef OS_GAZETTEER_HPP
#define OS_GAZETTEER_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace photoalbum
{
    namespace os
    {
        /*
         * Import gazetteer data from a colon separated file to a SQLite
         * database.
         */
        int gazetteer(int argc, const char *argv[]);
    }
}

#endif

