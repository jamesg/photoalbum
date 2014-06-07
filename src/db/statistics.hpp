#ifndef DB_STATISTICS_HPP
#define DB_STATISTICS_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "json/map_accessor.hpp"

namespace sqlite
{
    class connection;
}

namespace photograph
{
    struct statistics : public json::map_accessor
    {
        statistics(json::object& o) :
            map_accessor(o)
        {
        }
        /*
         * The total number of photographs stored in the database.
         */
        int& total_count() const { return get_int("total_count"); }
    };

    namespace db
    {
        void get(sqlite::connection&, const statistics&);
    }
}

BOOST_FUSION_ADAPT_STRUCT(
        photograph::statistics,
        (int&, total_count())
        )

#endif

