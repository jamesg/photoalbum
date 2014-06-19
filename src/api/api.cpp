/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "api.hpp"

#include <locale>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/vector.hpp>

#include "json/map_accessor.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

#include "db/photograph.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

namespace
{
    typedef photograph::photograph photograph_t;

    struct location : json::map_accessor
    {
        location(json::object& o) : map_accessor(o)
        {
        }
        std::string& str() const { return get_string("location"); }
    };

    struct tag : json::map_accessor
    {
        tag(json::object& o) : map_accessor(o)
        {
        }
        std::string& str() const { return get_string("tag"); }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        ::location,
        (std::string&, str())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::tag,
        (std::string&, str())
        )

BOOST_FUSION_ADAPT_STRUCT(
        photograph::photograph,
        (int&,         id())
        (std::string&, title())
        (std::string&, caption())
        (std::string&, filename())
        (std::string&, location())
        (std::string&, taken())
        )

void photograph::api::recent_photographs(
        jsonrpc::request& request,
        jsonrpc::result& result,
        sqlite::connection& db
        )
{
    json::list photographs;
    int limit = 36;
    sqlite::select<photograph_t>(
            db,
            "SELECT DISTINCT photograph_id, title, caption, filename, location, taken "
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
    sqlite::select< ::location>(
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
    sqlite::select< ::tag>(
            db,
            "SELECT DISTINCT tag FROM photograph_tagged "
            "ORDER BY tag ",
            sqlite::empty_row(),
            list
            );
    result.data() = list;
}

