/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api/auth.hpp"

#include <boost/nondet_random.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>

#include "db/auth.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

void photoalbum::api::auth::login(
    jsonrpc::request& request,
    jsonrpc::result& result,
    sqlite::connection& conn
    )
{
    const std::string
        username = request.params().get<std::string>(0),
        password = request.params().get<std::string>(1);

    json::object user_o;
    photoalbum::auth::user user(user_o);
    db::auth::username_user(conn, username, user);

    if(user.id() && user.password() == password)
    {
        // Generate a token.
        std::ostringstream oss;
        boost::random_device rng;
        static const std::string chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
        boost::variate_generator<boost::random_device&, boost::uniform_int<>>
            gen(rng, boost::uniform_int<>(0, chars.size()-1));
        for(int i = 0; i < 64; ++i)
            oss << chars[gen()];
        const std::string token = oss.str();

        // Store the token.
        db::auth::issue_token(token, user.id(), conn);
        result.data() = token;
    }
    else if(user.id())
    {
        result.error() = "Incorrect password.";
    }
    else
    {
        result.error() = "Unknown user.";
    }
}

void photoalbum::api::auth::logout(
    jsonrpc::request& request,
    jsonrpc::result&,
    sqlite::connection& conn
    )
{
    const std::string token = request.params().get<std::string>(0);
    db::auth::invalidate(token, conn);
}

void photoalbum::api::auth::token_valid(
    jsonrpc::request& request,
    jsonrpc::result& result,
    sqlite::connection& conn
    )
{
    const std::string token = request.params().get<std::string>(0);
    result.data() = db::auth::token_valid(token, conn);
}

void photoalbum::api::auth::update_user(
    jsonrpc::request& request,
    jsonrpc::result& result,
    sqlite::connection& auth_db
    )
{
    json::object user_o;
    photoalbum::auth::user user(user_o);
    db::auth::token_user(auth_db, request.token(), user);

    json::object db_user_o;
    photoalbum::auth::user db_user(db_user_o);
    db::get_by_id(user.id(), auth_db, db_user);

    photoalbum::auth::user new_user(request.params()[0]);

    if(user.username() != "root" && user.id() != new_user.id())
        result.error() = "Not authorised.";
    else if(db_user.password() != new_user.get_string("current_password"))
        result.error() = "Current password doesn't match.";
    else if(db_user.username() != new_user.username())
        result.error() = "Changing usernames is not allowed.";
    else if(new_user.password().length() == 0)
        result.error() = "Empty passwords are not allowed.";
    else
        db::update(new_user, auth_db);
}

void photoalbum::api::auth::logged_in_user(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& auth_db
        )
{
    photoalbum::auth::user user(result.data());
    db::auth::token_user(auth_db, request.token(), user);
    user.password() = "";
}

