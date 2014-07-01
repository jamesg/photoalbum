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
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"
#include "sqlite/update.hpp"

namespace photoalbum
{
    const char markdown_id_str[]     = "markdown_id";
    const char note_id_str[]         = "note_id";
    const char note_version_id_str[] = "note_version_id";
}

void photoalbum::db::note::create(sqlite::connection& conn)
{
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS note ( "
            "    note_id                 INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    created                 VARCHAR NOT NULL, "
            "    title                   INTEGER NOT NULL "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS note_version ( "
            "    note_version_id         INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    note_id                 INTEGER NOT NULL, "
            "    markdown_id             INTEGER NOT NULL, "
            "    phase                   INTEGER NOT NULL, "
            "    modified                INTEGER NOT NULL, "
            "    FOREIGN KEY(note_id) REFERENCES note(note_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    FOREIGN KEY(markdown_id) REFERENCES markdown(markdown_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    UNIQUE(note_id, phase) "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TRIGGER IF NOT EXISTS note_version_update_modified "
            "AFTER UPDATE ON note_version "
            "FOR EACH ROW "
            "    BEGIN "
            "        UPDATE note_version "
            "        SET modified = '2014-04-16' "
            "        WHERE note_version_id = NEW.note_version_id; "
            "    END ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TRIGGER IF NOT EXISTS note_version_delete_markdown "
            "AFTER DELETE ON note_version "
            "FOR EACH ROW "
            "    BEGIN "
            "        DELETE FROM markdown "
            "        WHERE markdown_id = OLD.markdown_id; "
            "    END ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS note_version_uses_photograph ( "
            "    note_version_id         INTEGER NOT NULL, "
            "    photograph_id           INTEGER NOT NULL, "
            "    filename                VARCHAR NOT NULL, "
            "    FOREIGN KEY(note_version_id) REFERENCES note_version(note_version_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
            "    FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
            "    ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS markdown ( "
            "    markdown_id             INTEGER PRIMARY KEY AUTOINCREMENT, "
            "    data                    VARCHAR NOT NULL DEFAULT '' "
            "    ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE VIEW IF NOT EXISTS draft_note AS "
            "    SELECT note_id, created, modified, data "
            "    FROM note NATURAL JOIN note_version NATURAL JOIN markdown "
            "    WHERE phase = 0 ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE VIEW IF NOT EXISTS published_note AS "
            "    SELECT note_id, created, modified, data "
            "    FROM note NATURAL JOIN note_version NATURAL JOIN markdown "
            "    WHERE phase = 1 ",
            sqlite::empty_row(),
            conn
            );
}

int photoalbum::db::insert(
        const photoalbum::note& note_,
        sqlite::connection& conn
        )
{
    return sqlite::insert(
            "note",
            { boost::optional<const char*>(), "title", "created" },
            note_,
            conn
            );
}

void photoalbum::db::update(
        const photoalbum::note& note_,
        sqlite::connection& conn
        )
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

void photoalbum::db::get(
        int id,
        sqlite::connection& conn,
        const photoalbum::note& note_
        )
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
    sqlite::get_list<photoalbum::note>(
            conn,
            "note",
            { note_id_str, "title", "created" },
            list
            );
}

void photoalbum::db::get_published_notes(
        sqlite::connection& conn,
        json::list& list
        )
{
    sqlite::select<photoalbum::note>(
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

