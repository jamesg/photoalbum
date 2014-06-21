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

void photograph::api::auth::login(
    jsonrpc::request& request,
    jsonrpc::result& result,
    sqlite::connection& conn
    )
{
    const std::string
        username = request.params().get<std::string>(0),
        password = request.params().get<std::string>(1);

    json::object user_o;
    photograph::auth::user user(user_o);
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

void photograph::api::auth::logout(
    jsonrpc::request& request,
    jsonrpc::result&,
    sqlite::connection& conn
    )
{
    const std::string token = request.params().get<std::string>(0);
    db::auth::invalidate(token, conn);
}

void photograph::api::auth::token_valid(
    jsonrpc::request& request,
    jsonrpc::result& result,
    sqlite::connection& conn
    )
{
    const std::string token = request.params().get<std::string>(0);
    result.data() = db::auth::token_valid(token, conn);
}

