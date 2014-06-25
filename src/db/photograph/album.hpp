#ifndef DB_ALBUM_HPP
#define DB_ALBUM_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "db/has_id.hpp"

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    extern const char album_id_cstr[];
    /*!
     */
    struct album : public has_id<album_id_cstr>
    {
        album(json::object& m) :
            has_id(m)
        {
        }
        std::string& name() const { return get_string("name"); }
        std::string& caption() const { return get_string("caption"); }
        std::string& fromdate() const { return get_string("fromdate"); }
        std::string& todate() const { return get_string("todate"); }
    };

    namespace db
    {
        int insert(const album&, sqlite::connection& db);
        void update(const album&, sqlite::connection& db);
        void get_by_id(int id, sqlite::connection& db, const album&);
        void get_album_list(sqlite::connection& db, json::list&);
    }
}

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::album,
        (int&,         id())
        (std::string&, name())
        (std::string&, caption())
        (std::string&, fromdate())
        (std::string&, todate())
        )

#endif

