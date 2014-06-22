/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "note.hpp"

#include <boost/fusion/include/vector.hpp>

#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/get_list.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "sqlite/update.hpp"

namespace photoalbum
{
    const char markdown_id_str[]     = "markdown_id";
    const char note_id_str[]         = "note_id";
    const char note_version_id_str[] = "note_version_id";
}

int photoalbum::db::insert(const note& note_, sqlite::connection& conn)
{
    return sqlite::insert(
            "note",
            { boost::optional<const char*>(), "title", "created" },
            note_,
            conn
            );
}

void photoalbum::db::update(const note& note_, sqlite::connection& conn)
{
    sqlite::update(
            "note",
            { "note_id", "title", "created" },
            note_,
            conn
            );
}

int photoalbum::db::insert(const markdown_data& data, sqlite::connection& conn)
{
    return sqlite::insert(
            "markdown",
            { boost::optional<const char*>(), "data" },
            data,
            conn
            );
}

void photoalbum::db::update(const markdown_data& data, sqlite::connection& conn)
{
    sqlite::update(
            "markdown",
            { markdown_id_str, "data" },
            data,
            conn
            );
}

int photoalbum::db::insert(const note_version& data, sqlite::connection& conn)
{
    return sqlite::insert(
            "note_version",
            {
                boost::optional<const char*>(),
                note_id_str,
                markdown_id_str,
                "phase",
                "modified"
            },
            data,
            conn
            );
}

void photoalbum::db::delete_note(int note_id, sqlite::connection& conn)
{
    sqlite::devoid(
            "DELETE FROM note WHERE note_id = ? ",
            boost::fusion::vector<int>(note_id),
            conn
            );
}

void photoalbum::db::delete_note_version(
        int                   note_id,
        note_version::phase_t phase,
        sqlite::connection&   conn
        )
{
    sqlite::devoid(
            "DELETE FROM note_version WHERE note_id = ? AND phase = ? ",
            boost::fusion::vector<int, int>(note_id, phase),
            conn
            );
}

void photoalbum::db::get(int id, sqlite::connection& conn, const note& note_)
{
    sqlite::get_by_id(
            "note",
            { note_id_str, "title", "created" },
            id,
            conn,
            note_
            );
}

void photoalbum::db::get_note_list(sqlite::connection& conn, json::list& list)
{
    sqlite::get_list<note>(
            conn,
            "note",
            { note_id_str, "title", "created" },
            list
            );
}

void photoalbum::db::get_published_notes(sqlite::connection& conn, json::list& list)
{
    sqlite::select<note>(
            conn,
            "SELECT note_id, title, created "
            "FROM note "
            "NATURAL JOIN note_version "
            "WHERE phase = ? ",
            boost::fusion::vector<int>(note_version::published),
            list
            );
}

void photoalbum::db::get(
        int note_id,
        note_version::phase_t phase_,
        sqlite::connection& conn,
        const markdown_data& data
        )
{
    json::list l;
    sqlite::select<markdown_data>(
            conn,
            "SELECT markdown_id, data "
            "FROM markdown "
            "NATURAL JOIN note_version "
            "WHERE note_id = ? AND phase = ? ",
            boost::fusion::vector<int, int>(note_id, phase_),
            l
            );
    if(l.size())
        data.get_object() = l[0];
}

void photoalbum::db::get(
        int                  markdown_id,
        sqlite::connection&  conn,
        const markdown_data& data
        )
{
    sqlite::get_by_id(
            "markdown",
            { markdown_id_str, "data" },
            markdown_id,
            conn,
            data
            );
}

void photoalbum::db::get(
        int id,
        note_version::phase_t phase_,
        sqlite::connection& conn,
        const note_version& data
        )
{
    json::list l;
    sqlite::select<note_version>(
            conn,
            "SELECT note_version_id, note_id, markdown_id, phase, modified "
            "FROM markdown NATURAL JOIN note_version "
            "WHERE note_version_id = ? AND phase = ? ",
            boost::fusion::vector<int, int>(id, phase_),
            l
            );
    if(l.size())
        data.get_object() = l[0];
}

void photoalbum::db::get(
        int note_version_id,
        sqlite::connection& conn,
        const note_version& data
        )
{
    sqlite::get_by_id(
            "note_version",
            {
                note_version_id_str,
                note_id_str,
                "markdown_id",
                "phase",
                "modified"
            },
            note_version_id,
            conn,
            data
            );
}

void photoalbum::db::get(
        int                 note_id,
        sqlite::connection& conn,
        const               published_note& note_
        )
{
    try
    {
        json::list l;
        sqlite::select<published_note>(
                conn,
                "SELECT created, modified, markdown "
                "FROM note "
                "NATURAL JOIN note_version "
                "NATURAL JOIN markdown "
                "WHERE phase = ? AND note_id = ? ",
                boost::fusion::vector<int, int>(
                    note_version::published,
                    note_id
                    ),
                l
                );
        if(l.size())
            note_.get_object() = l[0];
    }
    catch(const std::exception&)
    {
    }
}

