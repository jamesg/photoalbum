#ifndef DB_HAS_ID_HPP
#define DB_HAS_ID_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/map_accessor.hpp"

namespace photograph
{
    /*
     * Generic type for database types that have integer ids.
     */
    template<const char *Tid_field>
    struct has_id : public json::map_accessor
    {
        has_id(json::object& m) :
            json::map_accessor(m)
        {
        }
        static std::string id_field() { return Tid_field; }
        int& id() const { return get_int(Tid_field); }
    };
}

#endif

