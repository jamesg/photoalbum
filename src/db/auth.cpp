/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "db/auth.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

void photograph::db::auth::create(sqlite::connection& conn)
{
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS auth_token ( "
            " token VARCHAR PRIMARY KEY "
            " ) ",
            sqlite::empty_row(),
            conn
            );
}

bool photograph::db::auth::token_valid(
    const std::string& token,
    sqlite::connection& conn
    )
{
    sqlite::rowset<std::string> tokens;
    sqlite::select(
            conn,
            "SELECT token FROM auth_token WHERE token = ?",
            sqlite::row<std::string>(token),
            tokens
            );
    return !tokens.empty();
}

void photograph::db::auth::issue_token(
    const std::string& token,
    sqlite::connection& conn
    )
{
    sqlite::insert(
            "auth_token",
            { "token" },
            sqlite::row<std::string>(token),
            conn
            );
}

void photograph::db::auth::invalidate(
    const std::string& token,
    sqlite::connection& conn
    )
{
    sqlite::devoid(
            "DELETE FROM auth_token WHERE token = ? ",
            sqlite::row<std::string>(token),
            conn
            );
}

