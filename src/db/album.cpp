/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "album.hpp"

#include <boost/fusion/include/vector.hpp>
#include <boost/optional.hpp>

#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "sqlite/step.hpp"
#include "sqlite/update.hpp"

namespace photoalbum
{
    // These strings need to be stored in one place so that there is only one
    // instantiation of has_id for each database type.
    const char album_id_cstr[] = "album_id";
}

int photoalbum::db::insert(const album& al, sqlite::connection& db)
{
    json::list albums;
    sqlite::select<album>(
            db,
            "SELECT album_id, name, caption, fromdate, todate FROM album "
            "WHERE name = ? ",
            boost::fusion::vector<std::string>(al.name()),
            albums
            );
    if(albums.size())
        return -1;
    return sqlite::insert(
            "album",
            {
                boost::optional<const char*>(),
                "name",
                "caption",
                "fromdate",
                "todate"
            },
            al,
            db
            );
}

void photoalbum::db::update(const album& al, sqlite::connection& db)
{
    sqlite::update(
            "album",
            { "album_id", "name", "caption", "fromdate", "todate" },
            al,
            db
            );
}

void photoalbum::db::get_by_id(int id, sqlite::connection& db, const album& al)
{
    sqlite::get_by_id(
            "album",
            { "album_id", "name", "caption", "fromdate", "todate" },
            id,
            db,
            al
            );
}

void photoalbum::db::get_album_list(sqlite::connection& db, json::list& list)
{
    sqlite::select<album>(
            db,
            "SELECT album_id, name, caption, fromdate, todate FROM album "
            "ORDER BY name ASC",
            boost::fusion::vector<>(),
            list
            );
}

