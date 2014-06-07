#ifndef PHOTOGRAPH_URI_URI_TYPE_HPP
#define PHOTOGRAPH_URI_URI_TYPE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <functional>

#include "mongoose.h"

namespace photograph
{
    namespace uri
    {
        typedef std::function<int(mg_connection*, mg_event)> uri_type;
    }
}

#endif

