/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api.hpp"

#include <locale>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

void photograph::api::markdown_data(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& conn
        )
{
    try
    {
        int id = request.params().get<int>(0);
        note_version::phase_t phase = static_cast<note_version::phase_t>(
                request.params().get<int>(1)
                );
        db::get(id, phase, conn, ::photograph::markdown_data(result.data()));
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::update_markdown_data(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        ::photograph::markdown_data md(request.params()[0]);
        db::update(md, conn);
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::note(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        db::get(
                request.params().get<int>(0),
                conn,
                ::photograph::note(result.data())
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::note_list(
                jsonrpc::request&,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        json::list notes;
        db::get_note_list(conn, notes);
        result.data() = notes;
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::note_version(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        db::get(
                request.params().get<int>(0),
                conn,
                ::photograph::note_version(result.data())
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::note_phase_version(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        db::get(
                request.params().get<int>(0),
                static_cast< ::photograph::note_version::phase_t>(
                    request.params().get<int>(1)
                    ),
                conn,
                ::photograph::note_version(result.data())
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::insert_note(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& conn
        )
{
    try
    {
        db::insert(::photograph::note(request.params()[0]), conn);
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::create_draft_note(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& conn
        )
{
    try
    {
        // TODO: transaction
        json::object note_o;
        ::photograph::note note_(note_o);
        note_.title() = request.params().get<std::string>(0);

        boost::gregorian::date today =
            boost::date_time::day_clock<boost::gregorian::date>::local_day();
        note_.created() = boost::gregorian::to_iso_extended_string(today);

        int note_id = db::insert(note_, conn);

        json::object markdown_data_o;
        int markdown_id = db::insert(
                ::photograph::markdown_data(markdown_data_o),
                conn
                );

        json::object note_version_o;
        ::photograph::note_version note_version_(note_version_o);
        note_version_.note_id() = note_id;
        note_version_.markdown_id() = markdown_id;
        note_version_.modified() = note_.created();
        note_version_.phase() = ::photograph::note_version::draft;
        int note_version_id = db::insert(note_version_, conn);

        json::map_accessor result_data(result.data());
        result_data.get_int("note_id")         = note_id;
        result_data.get_int("note_version_id") = note_version_id;
        result_data.get_int("markdown_id")     = markdown_id;
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::delete_note(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        ::photograph::db::delete_note(request.params().get<int>(0), conn);
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::published_notes(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        json::list notes;
        db::get_published_notes(conn, notes);
        result.data() = notes;
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::publish_note(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        const int note_id = request.params().get<int>(0);

        // TODO: transaction

        // Delete any old note versions.
        db::delete_note_version(
                note_id,
                ::photograph::note_version::published,
                conn
                );

        // Copy the markdown data to a new markdown row.
        sqlite::devoid(
                "INSERT INTO markdown(data) "
                "SELECT data "
                "FROM markdown NATURAL JOIN note_version "
                "WHERE note_id = ? AND phase = ? ",
                boost::fusion::vector<int, int>(
                    note_id,
                    note_version::draft
                    ),
                conn
                );
        // Copy the note version.
        boost::gregorian::date today =
            boost::date_time::day_clock<boost::gregorian::date>::local_day();
        sqlite::devoid(
                "INSERT INTO note_version( "
                " note_id, markdown_id, phase, modified) "
                "VALUES(?, last_insert_rowid(), ?, ?) ",
                boost::fusion::vector<int, int, std::string>(
                    note_id,
                    ::photograph::note_version::published,
                    boost::gregorian::to_iso_extended_string(today)
                    ),
                conn
                );
        // Copy photographs used by the note version.
        sqlite::devoid(
                "INSERT INTO note_version_uses_photograph( "
                " note_version_id, photograph_id, filename "
                " ) "
                "SELECT note_version_id AS a, photograph_id AS b, "
                " filename AS c "
                "FROM note_version_uses_photograph "
                "NATURAL JOIN note_version "
                "WHERE note_id = ? AND phase = ? ",
                boost::fusion::vector<int, int>(
                    note_id,
                    ::photograph::note_version::draft
                    ),
                conn
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

void photograph::api::published_note(
                jsonrpc::request&   request,
                jsonrpc::result&    result,
                sqlite::connection& conn
                )
{
    try
    {
        db::get(
                request.params().get<int>(0),
                conn,
                ::photograph::published_note(result.data())
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Unknown error";
    }
}

