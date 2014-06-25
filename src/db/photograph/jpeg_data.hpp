#ifndef DB_JPEG_DATA_HPP
#define DB_JPEG_DATA_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "json/map_accessor.hpp"

#include "db/photograph.hpp"

namespace photoalbum
{
    /*!
     * Base64 JPEG data for a photograph.
     */
    struct jpeg_data : public has_id<photograph_id_cstr>
    {
        jpeg_data(json::object& m) :
            has_id<photograph_id_cstr>(m)
        {
        }
        /*!
         * Base64 encoded data
         */
        std::string& data() const { return get_string("data"); }
    };

    /*!
     * Raw JPEG data for a photograph.
     */
    struct jpeg_data_db : public has_id<photograph_id_cstr>
    {
        jpeg_data_db(json::object& m) :
            has_id<photograph_id_cstr>(m)
        {
        }

        std::vector<unsigned char> data;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::jpeg_data,
        (int&,          id())
        (std::string&,  data())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::jpeg_data_db,
        (int&,                id())
        (std::vector<unsigned char>, data)
        )

namespace photoalbum
{
    namespace db
    {
        void get_by_id(const int, sqlite::connection&, const jpeg_data&);
        void get_by_id(const int, sqlite::connection&, jpeg_data_db&);
        void insert(const jpeg_data&, sqlite::connection&);
        void insert(const jpeg_data_db&, sqlite::connection&);
    }
}

#endif

