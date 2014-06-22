#ifndef PHOTOGRAPH_URI_INSERT_PHOTOGRAPH
#define PHOTOGRAPH_URI_INSERT_PHOTOGRAPH

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "mongoose.h"

namespace photoalbum
{
    namespace uri
    {
        int insert_photograph(mg_connection*, mg_event);
    }
}


#endif

