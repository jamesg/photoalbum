/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "map.hpp"

#include <boost/fusion/include/vector.hpp>

#include "base64/base64.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/devoid.hpp"
#include "sqlite/get_by_id.hpp"
#include "sqlite/insert.hpp"
#include "sqlite/select.hpp"
#include "sqlite/step.hpp"
#include "sqlite/temporary_table.hpp"

namespace photograph
{
    namespace map
    {
        const char gazetteer_record_id_str[] = "seq";
    }
}

void photograph::db::get(
    sqlite::connection& conn,
    const std::string& region,
    int eastings,
    int northings,
    const map::tile& out
    )
{
    json::list l;
    sqlite::select<map::tile>(
            conn,
            "SELECT region, eastings, northings "
            "FROM tile "
            "WHERE region = ? AND eastings = ? AND northings = ? ",
            boost::fusion::vector<std::string, int, int>(
                region, eastings, northings
                ),
            l
            );
    if(l.size())
        out.get_object() = l[0];
}

void photograph::db::get(
    sqlite::connection& conn,
    const std::string& region,
    int eastings,
    int northings,
    map::tile_data_db& data
    )
{
    std::vector<map::tile_data_db> l;
    sqlite::select<map::tile_data_db>(
            conn,
            "SELECT region, eastings, northings, data "
            "FROM tile_data "
            "WHERE region = ? AND eastings = ? AND northings = ? ",
            boost::fusion::vector<std::string, int, int>(
                region, eastings, northings
                ),
            l
            );
    if(l.size())
        data = l.at(0);
}

void photograph::db::get(
        sqlite::connection& conn,
        const std::string& region,
        int eastings,
        int northings,
        const map::tile_data& out
        )
{
    map::tile_data_db data;
    get(conn, region, eastings, northings, data);
    out.region() = data.region;
    out.eastings() = data.eastings;
    out.northings() = data.northings;
    out.data() = base64::encode(
            reinterpret_cast<const unsigned char*>(&(data.data[0])),
            data.data.size()
            );
}

void photograph::db::insert(
    const photograph::map::tile_data& td,
    sqlite::connection& conn
    )
{
    sqlite::insert(
            "tile_data",
            { "region", "eastings", "northings", "data" },
            boost::fusion::vector<std::string, int, int, std::string>(
                td.region(),
                td.eastings(),
                td.northings(),
                td.data()
                ),
            conn
            );
}

void photograph::db::insert(
    photograph::map::tile_data_db& td,
    sqlite::connection& conn
    )
{
    sqlite3_stmt *stmt;
    sqlite3_prepare(
            conn.handle(),
            "INSERT INTO tile_data(region, eastings, northings, data) "
            "VALUES (?, ?, ?, ?) ",
            -1,
            &stmt,
            nullptr
            );
    sqlite::bind(1, td.region, stmt);
    sqlite::bind(2, td.eastings, stmt);
    sqlite::bind(3, td.northings, stmt);
    sqlite::bind(4, &(td.data[0]), td.data.size(), stmt);
    sqlite::step(stmt, conn);
    sqlite3_finalize(stmt);
}

void photograph::db::get(
        sqlite::connection&          conn,
        int                          seq,
        const map::gazetteer_record& out
        )
{
    sqlite::get_by_id(
            "gazetteer",
            {
                "seq",
                "km_ref",
                "def_nam",
                "tile_ref",
                "lat_deg",
                "lat_min",
                "long_deg",
                "long_min",
                "north",
                "east",
                "gmt",
                "co_code",
                "county",
                "full_county",
                "f_code",
                "e_date",
                "update_co",
                "sheet_1",
                "sheet_2",
                "sheet_3"
            },
            seq,
            conn,
            out
            );
}

void photograph::db::insert(
        const map::gazetteer_record& in,
        sqlite::connection&          conn
        )
{
    sqlite::insert(
            "gazetteer",
            {
                "seq",
                "km_ref",
                "def_nam",
                "tile_ref",
                "lat_deg",
                "lat_min",
                "long_deg",
                "long_min",
                "north",
                "east",
                "gmt",
                "co_code",
                "county",
                "full_county",
                "f_code",
                "e_date",
                "update_co",
                "sheet_1",
                "sheet_2",
                "sheet_3"
            },
            in,
            conn
            );
}

void photograph::db::insert_gazetteer_records(
        json::list&         in,
        sqlite::connection& conn
        )
{
    sqlite::insert<map::gazetteer_record>(
            "gazetteer",
            {
                "seq",
                "km_ref",
                "def_nam",
                "tile_ref",
                "lat_deg",
                "lat_min",
                "long_deg",
                "long_min",
                "north",
                "east",
                "gmt",
                "co_code",
                "county",
                "full_county",
                "f_code",
                "e_date",
                "update_co",
                "sheet_1",
                "sheet_2",
                "sheet_3"
            },
            in,
            conn
            );
}

void photograph::db::clear_gazetteer(sqlite::connection& conn)
{
    sqlite::devoid("DELETE FROM gazetteer", boost::fusion::vector<>(), conn);
}

void photograph::db::clear_gazetteer(
        const std::vector<std::string>& regions,
        sqlite::connection&          conn
        )
{
    // Regions of interest.
    sqlite::temporary_table regions_table("tmp_regions", "code VARCHAR", conn);
    for(const std::string& region : regions)
    {
        sqlite::insert(
                regions_table.name(),
                { "code" },
                boost::fusion::vector<std::string>(region),
                conn
                );
    }

    std::ostringstream sql;
    sql << "DELETE FROM gazetteer ";
    sql << "WHERE substr(km_ref, 0, 3) IN ( ";
    sql << "SELECT code FROM " << regions_table.name();
    sql << ") ";
    sqlite::devoid(
            sql.str(),
            boost::fusion::vector<>(),
            conn
            );
}

void photograph::db::delete_tile_data(
        const std::string& region,
        int eastings,
        int northings,
        sqlite::connection& conn
        )
{
    sqlite::devoid(
            "DELETE FROM tile_data "
            "WHERE region = ? "
            "AND eastings = ? "
            "AND northings = ? ",
            boost::fusion::vector<std::string, int, int>(
                region, eastings, northings
                ),
            conn
            );
}

void photograph::db::search_gazetteer(
        sqlite::connection& conn,
        const std::string& search_term,
        json::list& out
        )
{
    std::ostringstream oss;
    oss << "%" << search_term << "%";
    sqlite::select<map::gazetteer_record>(
            conn,
            "SELECT seq, km_ref, def_nam, lat_deg, lat_min, long_deg, "
            "long_min, north, east, gmt, co_code, county, full_county, f_code, "
            "e_date, update_co, sheet_1, sheet_2, sheet_3 "
            "FROM gazetteer "
            "WHERE def_nam LIKE ? ",
            boost::fusion::vector<std::string>(oss.str()),
            out
            );
}

