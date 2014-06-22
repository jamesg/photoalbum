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
#include "sqlite/select.hpp"
#include "sqlite/update.hpp"

#include "db/album.hpp"

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

//void photoalbum::db::photoalbum::create(sqlite::connection& conn)
//{
//}

int photoalbum::db::insert(const photograph& photo, sqlite::connection& db)
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

    for(json::object p : photo.get_list("tags").objects)
        sqlite::insert(
                "photograph_tagged",
                { "photograph_id", "tag" },
                boost::fusion::vector<int, std::string>(
                    id,
                    boost::get<std::string>(p)
                    ),
                db
                );

    return id;
}

void photoalbum::db::update(const photograph& photo, sqlite::connection& db)
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
    for(json::object p : photo.get_list("tags").objects)
    {
        //std::cerr << "insert photograph_tagged: " << json::serialise_json(p) << std::endl;
        try
        {
            sqlite::insert(
                    "photograph_tagged",
                    { "photograph_id", "tag" },
                    boost::fusion::vector<int, std::string>(
                        photo.id(), json::cast<std::string>(p)
                        ),
                    db
                    );
        }
        catch(const std::exception& e)
        {
        }
    }
}

void photoalbum::db::get_by_id(
        int id,
        sqlite::connection& db,
        const photograph& photo
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
    sqlite::select<photograph_tagged>(
            db,
            "SELECT photograph_id, tag "
            "FROM photograph_tagged "
            "WHERE photograph_id = ?",
            boost::fusion::vector<int>(id),
            photo.get_list("tags")
            );
}

void photoalbum::db::get_photograph_list(
        sqlite::connection& db,
        json::list&         list
        )
{
    sqlite::get_list<photograph>(
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

