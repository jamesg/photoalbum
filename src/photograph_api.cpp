/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "photograph_api.hpp"

#include <locale>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"
#include "sqlite/select.hpp"

#include "photograph_db.hpp"

namespace
{
    typedef photograph::photograph photograph_t;
}

void photograph::api::recent_photographs(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    json::list photographs;
    int limit = 36;
    sqlite::select<photograph_t>(db,
            "SELECT DISTINCT photograph_id, title, caption, location, taken "
            "FROM photograph "
            "ORDER BY taken DESC "
            "LIMIT ? "
            "OFFSET 0 ",
            boost::fusion::vector<int>(limit),
            photographs
            );
    result.data() = photographs;
}

void photograph::api::locations(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list list;
    sqlite::select< ::photograph::location>(
           db,
           "SELECT DISTINCT location "
           "FROM photograph "
           "ORDER BY location ",
           boost::fusion::vector<>(),
           list
           );
    result.data() = list;
}

void photograph::api::tags(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list list;
    sqlite::get_list< ::photograph::tag>(
            db,
            "photograph_tagged",
            { "tag" },
            list
            );
    result.data() = list;
}

