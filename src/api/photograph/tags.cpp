#include "tags.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

#include "json/map_accessor.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

#include "db/photograph.hpp"

namespace
{
    struct tag : json::map_accessor
    {
        tag(json::object& o) : map_accessor(o)
        {
        }
        std::string& str() const { return get_string("tag"); }
        // The number of photographs to which this tag has been applied.
        int& count() const { return get_int("count"); }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        ::tag,
        (std::string&, str())
        )

void photoalbum::api::tags(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    tags_alphabetical(request, result, db);
}

void photoalbum::api::tags_alphabetical(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list list;
    sqlite::select< ::tag>(
            db,
            "SELECT DISTINCT tag, COUNT(photograph_id) AS count "
            "FROM photograph_tagged "
            "GROUP BY tag "
            "ORDER BY tag ",
            sqlite::empty_row(),
            list
            );
    result.data() = list;
}

void photoalbum::api::tags_popular(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    json::list list;
    sqlite::select< ::tag>(
            db,
            "SELECT DISTINCT tag, COUNT(photograph_id) AS count "
            "FROM photograph_tagged "
            "GROUP BY tag "
            "ORDER BY count DESC ",
            sqlite::empty_row(),
            list
            );
    result.data() = list;
}

