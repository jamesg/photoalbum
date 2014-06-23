/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "album.hpp"

#include <exception>
#include <locale>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>

#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

#include "db/photograph/album.hpp"
#include "db/photograph.hpp"

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::photograph,
        (int&,         id())
        (std::string&, title())
        (std::string&, caption())
        (std::string&, filename())
        (std::string&, location())
        (std::string&, taken())
        )

void photoalbum::api::add_photograph_to_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    int photograph_id, album_id;

    try
    {
        photograph_id = request.params().get<int>(0);
        album_id      = request.params().get<int>(1);
    }
    catch(const std::exception&)
    {
        result.error() = "Reading parameters";
        std::rethrow_exception(std::current_exception());
    }
    try
    {
        sqlite::insert(
                "photograph_in_album",
                { "photograph_id", "album_id" },
                sqlite::row<int, int>(photograph_id, album_id),
                db
                );
    }
    catch(const std::exception&)
    {
        result.error() = "Database error";
        std::rethrow_exception(std::current_exception());
    }
}

void photoalbum::api::album_list(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list album_list = json::list();
    db::get_album_list(db, album_list);
    result.data() = album_list;
}

void photoalbum::api::insert_album(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    db::insert(album(request.params()[0]), db);
}

void photoalbum::api::uncategorised_photographs(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    json::list photographs;
    sqlite::select< ::photoalbum::photograph>(
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

void photoalbum::api::update_album(
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

void photoalbum::api::delete_album(
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

