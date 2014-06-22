#ifndef API_STATISTICS_HPP
#define API_STATISTICS_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace jsonrpc
{
    class request;
    class result;
}

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace api
    {
        void statistics(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
    }
}

#endif

