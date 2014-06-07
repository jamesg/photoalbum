#ifndef PHOTOGRAPH_URI_HTTP_DATE_HPP
#define PHOTOGRAPH_URI_HTTP_DATE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

namespace boost
{
    namespace posix_time
    {
        class ptime;
    }
}

namespace photograph
{
    namespace uri
    {
        std::string http_date(const boost::posix_time::ptime&);
    }
}

#endif

