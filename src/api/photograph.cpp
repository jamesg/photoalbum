/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api.hpp"

#include <locale>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

#include "sqlite/row.hpp"

namespace
{
    typedef photograph::photograph photograph_t;
}

void photograph::api::photograph(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        int id = request.params().get<int>(0);
        json::object photo_obj = json::map();
        photograph_t photo(photo_obj);
        db::get_by_id(id, db, photo);
        result.data() = photo.get_object();
    }
    catch( const std::exception& e )
    {
        std::cerr << "In API function photograph: " << e.what() << std::endl;
    }
}

void photograph::api::delete_photograph(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        sqlite::devoid(
                "DELETE FROM photograph WHERE photograph_id = ?",
                boost::fusion::vector<int>(request.params().get<int>(0)),
                db
                );
    }
    catch( const std::exception& e )
    {
        result.error() = "Error updating photograph";
    }
}

void photograph::api::insert_photograph(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    int id;
    try
    {
        id = db::insert(::photograph::photograph(request.params()[0]), db);
    }
    catch( const std::exception& e )
    {
        result.error() = "Error inserting photograph details.";
    }
    json::map o = json::map();
    o.get_int("photograph_id") = id;
    result.data() = o;
}

void photograph::api::photograph_albums(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    json::list photographs = json::list();
    int pid = request.params().get<int>(0);
    sqlite::select<album>(
            db,
            "SELECT DISTINCT album_id, name, caption, fromdate, todate "
            "FROM photograph_in_album NATURAL JOIN album "
            "WHERE photograph_id = ? ",
            boost::fusion::vector<int>(pid),
            photographs
            );
    result.data() = photographs;
}

void photograph::api::photograph_list(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list photo_list = json::list();
    db::get_photograph_list(db, photo_list);
    result.data() = photo_list;
}

void photograph::api::photographs_in_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    int id = request.params().get<int>(0);
    json::list photographs = json::list();
    db::get_photographs_by_album(db, id, photographs);
    result.data() = photographs;
}

void photograph::api::remove_photograph_from_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        sqlite::devoid(
                "DELETE FROM photograph_in_album "
                "WHERE photograph_id = ? AND album_id = ?",
                sqlite::row<int, int>(
                    request.params().get<int>(0),
                    request.params().get<int>(1)
                    ),
                db
                );
    }
    catch( const std::exception& e )
    {
    }
}

void photograph::api::update_photograph(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        ::photograph::photograph photo(request.params()[0]);
        db::update(photo, db);
    }
    catch( const std::exception& e )
    {
        result.error() = "Error updating photograph";
    }
}

void photograph::api::photographs_with_tag(
            jsonrpc::request&   request,
            jsonrpc::result&    result,
            sqlite::connection& db
            )
{
    json::list photographs;
    sqlite::select< ::photograph::photograph>(
            db,
            "SELECT DISTINCT photograph.photograph_id, title, caption, "
            "filename, location, taken "
            "FROM photograph NATURAL JOIN photograph_tagged "
            "WHERE tag = ? ",
            boost::fusion::vector<std::string>(
                request.params().get<std::string>(0)
                ),
            photographs
            );
    result.data() = photographs;
}

void photograph::api::photographs_with_location(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list photographs;
    sqlite::select< ::photograph::photograph>(
            db,
            "SELECT DISTINCT "
            "photograph_id, title, caption, filename, location, taken "
            "FROM photograph "
            "WHERE location = ? ",
            boost::fusion::vector<std::string>(
                request.params().get<std::string>(0)
                ),
            photographs);
    result.data() = photographs;
}

