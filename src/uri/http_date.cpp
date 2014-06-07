/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "uri/http_date.hpp"

#include <sstream>

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string photograph::uri::http_date(const boost::posix_time::ptime& time)
{
    boost::local_time::local_time_facet* lf(
            new boost::local_time::local_time_facet("%a, %d %b %Y %H:%M:%S GMT")
            );
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), lf));
    ss << time;
    return ss.str();
}

