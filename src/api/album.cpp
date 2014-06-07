/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api.hpp"

#include <locale>

#include <boost/fusion/include/vector.hpp>

#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"

#include "photograph_db.hpp"

void photograph::api::add_photograph_to_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        int photograph_id = request.params().get<int>(0);
        int album_id      = request.params().get<int>(1);
        sqlite::insert(
                "photograph_in_album",
                { "photograph_id", "album_id" },
                boost::fusion::vector<int, int>(photograph_id, album_id),
                db
                );
    }
    catch( const std::exception& e )
    {
        result.error() = "Database error";
        throw e;
    }
}

void photograph::api::album_list(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list album_list = json::list();
    db::get_album_list(db, album_list);
    result.data() = album_list;
}

void photograph::api::insert_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    db::insert(album(request.params()[0]), db);
}

void photograph::api::uncategorised_photographs(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    json::list photographs;
    sqlite::select< ::photograph::photograph>(
            db,
            "SELECT DISTINCT "
            "photograph.photograph_id, photograph_in_album.album_id "
            "FROM photograph "
            "LEFT JOIN photograph_in_album "
            "ON photograph.photograph_id = photograph_in_album.photograph_id "
            "WHERE album_id IS NULL",
            boost::fusion::vector<>(),
            photographs
            );
    result.data() = photographs;
}

void photograph::api::update_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        album album(request.params()[0]);
        db::update(album, db);
    }
    catch(const std::exception& e)
    {
        result.error() = "Error updating album.";
    }
}

void photograph::api::delete_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    try
    {
        sqlite::devoid("DELETE FROM album WHERE album_id = ?",
                boost::fusion::vector<int>(request.params().get<int>(0)),
                db);
    }
    catch( const std::exception& e )
    {
        std::cerr << "In delete_album: " << e.what() << std::endl;
        result.error() = "Error deleting album";
    }
}

