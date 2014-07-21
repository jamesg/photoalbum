/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "photograph.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/optional.hpp>

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"
#include "sqlite/update.hpp"

#include "db/photograph/album.hpp"

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::photograph,
        (int&,         id())
        (std::string&, title())
        (std::string&, caption())
        (std::string&, filename())
        (std::string&, location())
        (std::string&, taken())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::photograph_in_album,
        (int&,         photograph_id())
        (int&,         album_id())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::photograph_tagged,
        (int&,         photograph_id())
        (std::string&, tag())
        )

namespace photoalbum
{
    const char photograph_id_cstr[] = "photograph_id";
}

void photoalbum::db::photograph::create(sqlite::connection& conn)
{
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph          ( "
            "    photograph_id           INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    title                   VARCHAR NOT NULL, "
            "    caption                 VARCHAR NULL, "
            "    filename                VARCHAR NOT NULL, "
            "    location                VARCHAR NULL, "
            "    taken                   VARCHAR NULL, "
            "    UNIQUE(photograph_id, title, caption, filename, location, taken) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS  jpeg_data           ( "
            "    photograph_id           INTEGER PRIMARY KEY, "
            "    data                    BLOB NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph(photograph_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(photograph_id, data) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS  album               ( "
            "    album_id                INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    name                    VARCHAR NOT NULL, "
            "    caption                 VARCHAR NULL, "
            "    fromdate                VARCHAR NULL, "
            "    todate                  VARCHAR NULL, "
            "    UNIQUE(album_id, name, caption, fromdate, todate) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph_tagged    ( "
            "    photograph_id           INTEGER NOT NULL, "
            "    tag                     VARCHAR NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(photograph_id, tag) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS  photograph_in_album   ( "
            "    photograph_id           INTEGER NOT NULL, "
            "    album_id                INTEGER NOT NULL, "
            "    FOREIGN KEY( photograph_id ) REFERENCES photograph( photograph_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    FOREIGN KEY( album_id )      REFERENCES album( album_id ) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(photograph_id, album_id) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
}

int photoalbum::db::insert(
        const photoalbum::photograph& photo,
        sqlite::connection& db
        )
{
    int id = sqlite::insert(
            "photograph",
            {
                boost::optional<const char*>(),
                "title",
                "caption",
                "filename",
                "location",
                "taken"
            },
            photo,
            db
            );

    for(int i = 0; i < photo.tags().size(); ++i)
        sqlite::insert(
            "photograph_tagged",
            { "photograph_id", "tag" },
            boost::fusion::vector<int, std::string>(
                id,
                photo.tags().get<std::string>(i)
                ),
            db
            );

    return id;
}

void photoalbum::db::update(
        const photoalbum::photograph& photo,
        sqlite::connection& db
        )
{
    sqlite::update(
            "photograph",
            {
                "photograph_id",
                "title",
                "caption",
                "filename",
                "location",
                "taken"
            },
            photo,
            db
            );

    // TODO: put this in a scoped transaction
    //
    sqlite::devoid(
            "DELETE FROM photograph_tagged WHERE photograph_id = ?",
            boost::fusion::vector<int>(photo.id()),
            db
            );
    for(int i = 0; i < photo.tags().size(); ++i)
        sqlite::insert(
            "photograph_tagged",
            { "photograph_id", "tag" },
            boost::fusion::vector<int, std::string>(
                photo.id(),
                photo.tags().get<std::string>(i)
                ),
            db
            );
}

void photoalbum::db::get_by_id(
        int id,
        sqlite::connection& db,
        const photoalbum::photograph& photo
        )
{
    sqlite::get_by_id(
            "photograph",
            {
                "photograph_id",
                "title",
                "caption",
                "filename",
                "location",
                "taken"
            },
            id,
            db,
            photo
            );

    // Also get the albums this photograph is a member of
    sqlite::select<album>(
            db,
            "SELECT album_id, name, caption, fromdate, todate "
            "FROM photograph_in_album "
            "NATURAL JOIN album "
            "WHERE photograph_id = ? "
            "ORDER BY name ASC ",
            boost::fusion::vector<int>(id),
            photo.get_list("albums")
            );

    // And the tags
    std::vector<sqlite::row<std::string>> tags;
    sqlite::select<sqlite::row<std::string>>(
            db,
            "SELECT tag FROM photograph_tagged "
            "WHERE photograph_id = ?",
            boost::fusion::vector<int>(id),
            tags
            );
    for(const sqlite::row<std::string>& row : tags)
        photo.tags().append(sqlite::column<0>(row));
}

void photoalbum::db::get_photograph_list(
        sqlite::connection& db,
        json::list&         list
        )
{
    sqlite::get_list<photoalbum::photograph>(
            db,
            "photograph",
            {
                "photograph_id",
                "title",
                "caption",
                "filename",
                "location",
                "taken"
            },
            list
            );
}

int photoalbum::db::insert(
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

void photoalbum::db::get_photographs_by_album(
        sqlite::connection& db,
        const int album_id,
        json::list& list
        )
{
    boost::fusion::vector<int> params(album_id);
    sqlite::select<photoalbum::photograph>(
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

