/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "photograph_db.hpp"

#include <boost/fusion/include/vector.hpp>
#include <boost/optional.hpp>

#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "sqlite/step.hpp"
#include "sqlite/update.hpp"

int photograph::db::insert(
        const photograph_in_album& photo_in_al,
        sqlite::connection& db
        )
{
    return sqlite::insert(
            "photograph_in_album",
            { "photograph_id", "album_id" },
            photo_in_al,
            db
            );
}

void photograph::db::get_photographs_by_album(
        sqlite::connection& db,
        const int album_id,
        json::list& list
        )
{
    boost::fusion::vector<int> params(album_id);
    sqlite::select<photograph>(
            db,
            "SELECT DISTINCT "
            "photograph_id, title, caption, filename, location, taken "
            "FROM photograph NATURAL JOIN photograph_in_album "
            "WHERE album_id = ? "
            "ORDER BY taken ASC ",
            params,
            list
            );
}

