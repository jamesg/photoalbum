#ifndef PHOTOGRAPH_DB_HPP
#define PHOTOGRAPH_DB_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "json/map_accessor.hpp"

#include "db/album.hpp"
#include "db/has_id.hpp"
#include "db/jpeg_data.hpp"
#include "db/map.hpp"
#include "db/note.hpp"
#include "db/photograph.hpp"

namespace photograph
{
    struct location : public json::map_accessor
    {
        location(const json::object& o) : json::map_accessor(o) { }
        std::string& value() const { return get_string("location"); }
    };

    struct tag : public json::map_accessor
    {
        tag(const json::object& o) : json::map_accessor(o) { }
        std::string& value() const { return get_string("tag"); }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        photograph::photograph_in_album,
        (int&,         photograph_id())
        (int&,         album_id())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photograph::photograph_tagged,
        (int&,         photograph_id())
        (std::string&, tag())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photograph::location,
        (std::string&, value())
    )

BOOST_FUSION_ADAPT_STRUCT(
        photograph::tag,
        (std::string&, value())
    )

/*
 * Section: Database access functions
 */

namespace photograph
{
    namespace db
    {
        int insert(const photograph_in_album&, sqlite::connection& db);
        void get_photographs_by_album(
                sqlite::connection& db,
                const int album_id,
                json::list& list
                );
    }
}

#endif

