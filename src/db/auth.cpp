/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "db/auth.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"
#include "sqlite/update.hpp"

const char photoalbum::auth::user_id_cstr[] = "user_id";

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::auth::user,
        (int&, id())
        (std::string&, username())
        (std::string&, password())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photoalbum::auth::user_details,
        (int&, id())
        (std::string&, first_name())
        (std::string&, last_name())
        )

void photoalbum::db::auth::create(sqlite::connection& conn)
{
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS auth_user ( "
            " user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            " username VARCHAR NOT NULL, "
            " password VARCHAR NOT NULL "
            " ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS auth_token ( "
            " token VARCHAR PRIMARY KEY, "
            " user_id INTEGER NOT NULL REFERENCES auth_user(user_id), "
            " created VARCHAR NOT NULL, "
            " expires VARCHAR NOT NULL, "
            " invalidated BOOLEAN NOT NULL DEFAULT false "
            " ) ",
            sqlite::empty_row(),
            conn
            );
    sqlite::devoid(
            "CREATE TABLE IF NOT EXISTS auth_user_details ( "
            " user_id PRIMARY KEY REFERENCES auth_user(user_id), "
            " first_name VARCHAR NOT NULL DEFAULT '', "
            " last_name VARCHAR NOT NULL DEFAULT '' "
            " ) ",
            sqlite::empty_row(),
            conn
            );

    json::list root_users;
    sqlite::select<photoalbum::auth::user>(
            conn,
            "SELECT user_id, username, password FROM auth_user "
            "WHERE username = 'root' ",
            sqlite::empty_row(),
            root_users
            );
    if(root_users.size() == 0)
        sqlite::devoid(
                "INSERT INTO auth_user(username, password) "
                "VALUES('root', 'root') ",
                sqlite::empty_row(),
                conn
                );
}

bool photoalbum::db::auth::token_valid(
    const std::string& token,
    sqlite::connection& conn
    )
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    sqlite::rowset<std::string> tokens;
    sqlite::select(
            conn,
            "SELECT token FROM auth_token "
            "WHERE token = ? "
            "AND expires > ? "
            "AND created <= ? "
            "AND NOT invalidated ",
            sqlite::row<std::string, std::string, std::string>(
                token,
                boost::posix_time::to_iso_extended_string(current_time),
                boost::posix_time::to_iso_extended_string(current_time)
                ),
            tokens
            );
    return !tokens.empty();
}

void photoalbum::db::auth::issue_token(
    const std::string&  token,
    const int           user_id,
    sqlite::connection& conn
    )
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    boost::posix_time::ptime expires_time(
            current_time +
            boost::posix_time::hours(24)
            );
    sqlite::insert(
            "auth_token",
            { "token", "user_id", "created", "expires" },
            sqlite::row<std::string, int, std::string, std::string>(
                    token,
                    user_id,
                    boost::posix_time::to_iso_extended_string(current_time),
                    boost::posix_time::to_iso_extended_string(expires_time)
                    ),
            conn
            );
}

void photoalbum::db::auth::invalidate(
    const std::string& token,
    sqlite::connection& conn
    )
{
    sqlite::devoid(
            "UPDATE auth_token SET invalidated = true WHERE token = ? ",
            sqlite::row<std::string>(token),
            conn
            );
}

void photoalbum::db::auth::token_user(
        sqlite::connection&     conn,
        const std::string&      token,
        photoalbum::auth::user& user
        )
{
    json::list users;
    sqlite::select<photoalbum::auth::user>(
            conn,
            "SELECT auth_user.user_id, username, password "
            "FROM auth_user, auth_token "
            "WHERE auth_user.user_id = auth_token.user_id "
            "AND token = ? ",
            sqlite::row<std::string>(token),
            users
            );
    if(users.size() == 1)
        user.get_object() = users.at(0);
}

void photoalbum::db::auth::username_user(
        sqlite::connection&     conn,
        const std::string&      username,
        photoalbum::auth::user& user
        )
{
    json::list users;
    sqlite::select<photoalbum::auth::user>(
            conn,
            "SELECT user_id, username, password FROM auth_user "
            "WHERE username = ? ",
            sqlite::row<std::string>(username),
            users
            );
    if(users.size() == 1)
        user.get_object() = users.at(0);
}

void photoalbum::db::auth::delete_old_tokens(sqlite::connection& conn)
{
    boost::posix_time::ptime current_time(
            boost::posix_time::second_clock::universal_time()
            );
    sqlite::devoid(
            "DELETE FROM auth_token WHERE expires < ? ",
            sqlite::row<std::string>(
                boost::posix_time::to_iso_extended_string(current_time)
                ),
            conn
            );
}

void photoalbum::db::get_by_id(
        int user_id,
        sqlite::connection& conn,
        const photoalbum::auth::user& user
        )
{
    sqlite::get_by_id(
            "auth_user",
            { "user_id", "username", "password" },
            user_id,
            conn,
            user
            );
}

void photoalbum::db::update(
        const photoalbum::auth::user& user,
        sqlite::connection& auth_db
        )
{
    sqlite::update(
            "auth_user",
            { boost::optional<const char*>(), "username", "password" },
            user,
            auth_db
            );
}

