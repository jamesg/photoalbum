/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jpeg_data.hpp"

#include "base64/base64.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/step.hpp"

void photoalbum::db::get_by_id(
        const int photograph_id,
        sqlite::connection& db,
        jpeg_data_db& data
        )
{
    json::object o = json::map();
    sqlite::get_by_id(
            "jpeg_data",
            { "photograph_id", "data" },
            photograph_id,
            db,
            data
            );
}

void photoalbum::db::get_by_id(
        const int photograph_id,
        sqlite::connection& db,
        const jpeg_data& data
        )
{
    json::object o = json::map();
    ::photoalbum::jpeg_data_db data_inter(o);

    get_by_id(photograph_id, db, data_inter);

    data.id() = data_inter.id();
    data.data() = base64::encode(
            reinterpret_cast<const unsigned char*>(&(data_inter.data[0])),
            data_inter.data.size()
            );
}

void photoalbum::db::insert(const jpeg_data& data, sqlite::connection& db)
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            db.handle(),
            "INSERT INTO jpeg_data(photograph_id, data) VALUES (?, ?)",
            -1,
            &stmt,
            nullptr
            );
    sqlite::bind(1, data.id(), stmt);
    std::string decoded = base64::decode(data.data());
    sqlite::bind(2, decoded.c_str(), decoded.length(), stmt);
    sqlite::step(stmt);
/*    if( sqlite3_step(stmt) != SQLITE_DONE )
    {
        std::cerr << "Error inserting image " << sqlite3_errmsg(db.handle()) << std::endl;
    }*/
    sqlite3_finalize(stmt);
}

void photoalbum::db::insert(const jpeg_data_db& data, sqlite::connection& db)
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            db.handle(),
            "INSERT INTO jpeg_data(photograph_id, data) VALUES (?, ?)",
            -1,
            &stmt,
            nullptr
            );
    sqlite::bind(1, data.id(), stmt);
    sqlite::bind(2, &(data.data[0]), data.data.size(), stmt);
    sqlite::step(stmt);
    sqlite3_finalize(stmt);
}

