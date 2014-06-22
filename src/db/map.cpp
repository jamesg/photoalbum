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
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"
#include "sqlite/step.hpp"
#include "sqlite/temporary_table.hpp"

namespace photoalbum
{
    namespace map
    {
        const char gazetteer_record_id_str[] = "seq";
    }
}

void photoalbum::db::map::create(sqlite::connection& conn)
{
    /*
     * Table for Ordnance Survey Street Map data.  Images are stored in PNG
     * format.
     */
    sqlite::devoid(
        "CREATE TABLE IF NOT EXISTS tile_data ( "
        "    region    VARCHAR, "
        "    eastings  INTEGER, "
        "    northings INTEGER, "
        "    data      BLOB NOT NULL, "
        "    PRIMARY KEY(region, eastings, northings) "
        ") ",
        sqlite::empty_row(),
        conn
        );
    /*
     * Create a view for obtaining tile information without the image data to
     * speed up queries where image data is not required.
     */
    sqlite::devoid(
        "CREATE VIEW IF NOT EXISTS tile AS "
        "SELECT region, eastings, northings FROM tile_data ",
        sqlite::empty_row(),
        conn
        );
    /*
     * Data for the OS 50k Gazetteer data set.  Field names are identical to
     * those in the documentation for the data set.
     */
    sqlite::devoid(
        "CREATE TABLE IF NOT EXISTS gazetteer ( "
        "    seq         INTEGER, "
        "    km_ref      VARCHAR, "
        "    def_nam     VARCHAR, "
        "    tile_ref    VARCHAR, "
        "    lat_deg     INTEGER, "
        "    lat_min     FLOAT, "
        "    long_deg    INTEGER, "
        "    long_min    FLOAT, "
        "    north       INTEGER, "
        "    east        INTEGER, "
        "    gmt         VARCHAR, "
        "    co_code     VARCHAR, "
        "    county      VARCHAR, "
        "    full_county VARCHAR, "
        "    f_code      VARCHAR, "
        "    e_date      VARCHAR, "
        "    update_co   VARCHAR, "
        "    sheet_1     INTEGER, "
        "    sheet_2     INTEGER, "
        "    sheet_3     INTEGER "
        ") ",
        sqlite::empty_row(),
        conn
        );
}

void photoalbum::db::get(
    sqlite::connection& conn,
    const std::string& region,
    int eastings,
    int northings,
    const photoalbum::map::tile& out
    )
{
    json::list l;
    sqlite::select<photoalbum::map::tile>(
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

void photoalbum::db::get(
    sqlite::connection& conn,
    const std::string& region,
    int eastings,
    int northings,
    photoalbum::map::tile_data_db& data
    )
{
    std::vector<photoalbum::map::tile_data_db> l;
    sqlite::select<photoalbum::map::tile_data_db>(
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

void photoalbum::db::get(
        sqlite::connection& conn,
        const std::string& region,
        int eastings,
        int northings,
        const photoalbum::map::tile_data& out
        )
{
    photoalbum::map::tile_data_db data;
    get(conn, region, eastings, northings, data);
    out.region() = data.region;
    out.eastings() = data.eastings;
    out.northings() = data.northings;
    out.data() = base64::encode(
            reinterpret_cast<const unsigned char*>(&(data.data[0])),
            data.data.size()
            );
}

void photoalbum::db::insert(
    const photoalbum::map::tile_data& td,
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

void photoalbum::db::insert(
    photoalbum::map::tile_data_db& td,
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

void photoalbum::db::get(
        sqlite::connection&          conn,
        int                          seq,
        const photoalbum::map::gazetteer_record& out
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

void photoalbum::db::insert(
        const photoalbum::map::gazetteer_record& in,
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

void photoalbum::db::insert_gazetteer_records(
        json::list&         in,
        sqlite::connection& conn
        )
{
    sqlite::insert<photoalbum::map::gazetteer_record>(
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

void photoalbum::db::clear_gazetteer(sqlite::connection& conn)
{
    sqlite::devoid("DELETE FROM gazetteer", boost::fusion::vector<>(), conn);
}

void photoalbum::db::clear_gazetteer(
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

void photoalbum::db::delete_tile_data(
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

void photoalbum::db::search_gazetteer(
        sqlite::connection& conn,
        const std::string& search_term,
        json::list& out
        )
{
    std::ostringstream oss;
    oss << "%" << search_term << "%";
    sqlite::select<photoalbum::map::gazetteer_record>(
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

