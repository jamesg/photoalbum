#ifndef DB_MAP_HPP
#define DB_MAP_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>

#include "json/map_accessor.hpp"

#include "db/has_id.hpp"

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace map
    {
        /*
         * Metadata for a tile.  The key consists of the region, eastings and
         * northings.
         */
        struct tile : public json::map_accessor
        {
            tile(json::object& m) : json::map_accessor(m) { }
            std::string& region() const { return get_string("region"); }
            int& eastings() const { return get_int("eastings"); }
            int& northings() const { return get_int("northings"); }
        };

        /*
         * Base64-encoded image data for a tile.  The key consists of the
         * region, eastings and northings.
         */
        struct tile_data : public json::map_accessor
        {
            tile_data(json::object& m) : json::map_accessor(m) { }
            std::string& region() const { return get_string("region"); }
            int& eastings() const { return get_int("eastings"); }
            int& northings() const { return get_int("northings"); }
            // Base64 encoded data
            std::string& data() const { return get_string("data"); }
        };

        /*
         * Internal tile data structure.  Contains raw data (not base64
         * encoded).
         */
        struct tile_data_db
        {
            std::string region;
            int eastings;
            int northings;
            std::vector<unsigned char> data;
        };

        extern const char gazetteer_record_id_str[];
        /*
         * Gazetteer record data structure.  Corresponds exactly to the
         * Ordnance Survey 50k Gazetteer data set.
         */
        struct gazetteer_record : public has_id<gazetteer_record_id_str>
        {
            int& seq() const { return get_int("seq"); }
            std::string& km_ref() const { return get_string("km_ref"); }
            std::string& def_nam() const { return get_string("def_nam"); }
            std::string& tile_ref() const { return get_string("tile_ref"); }
            int& lat_deg() const { return get_int("lat_deg"); }
            double& lat_min() const { return get_double("lat_min"); }
            int& long_deg() const { return get_int("long_deg"); }
            double& long_min() const { return get_double("long_min"); }
            int& north() const { return get_int("north"); }
            int& east() const { return get_int("east"); }
            std::string& gmt() const { return get_string("gmt"); }
            std::string& co_code() const { return get_string("co_code"); }
            std::string& county() const { return get_string("county"); }
            std::string& full_county() const { return get_string("full_county"); }
            std::string& f_code() const { return get_string("f_code"); }
            std::string& e_date() const { return get_string("e_date"); }
            std::string& update_co() const { return get_string("update_co"); }
            int& sheet_1() const { return get_int("sheet_1"); }
            int& sheet_2() const { return get_int("sheet_2"); }
            int& sheet_3() const { return get_int("sheet_3"); }

            gazetteer_record(json::object &m) : has_id<gazetteer_record_id_str>(m) { }
        };
    }

    namespace db
    {
        /*
         * Database functions specific to the maps component.
         */
        namespace map
        {
            /*
             * Create tables for map data if they do not exist in the database.
             * This database stores map information and images from the
             * Ordnance Survey Open Data archive.  When the database is used
             * from the photograph album server it is read only.
             */
            void create(sqlite::connection&);
        }
        /*
         * Get metadata about a tile.
         */
        void get(
                sqlite::connection&,
                const std::string& region,
                int eastings,
                int northings,
                const ::photoalbum::map::tile&
                );
        /*
         * Get image data for a tile.
         * The data will not be base64 encoded so is not intended to be
         * serialised.
         */
        void get(
                sqlite::connection&,
                const std::string& region,
                int eastings,
                int northings,
                ::photoalbum::map::tile_data_db&
                );
        /*
         * Get image data for a tile.
         * The data will be base64 encoded.
         */
        void get(
                sqlite::connection&,
                const std::string& region,
                int eastings,
                int northings,
                const ::photoalbum::map::tile_data&
                );

        void insert(
                const ::photoalbum::map::tile_data&,
                sqlite::connection&
                );

        void insert(
                ::photoalbum::map::tile_data_db&,
                sqlite::connection&
                );

        void get(
                sqlite::connection&,
                int seq,
                const ::photoalbum::map::gazetteer_record&
                );

        void insert(
                const ::photoalbum::map::gazetteer_record&,
                sqlite::connection&
                );

        void insert_gazetteer_records(json::list&, sqlite::connection&);

        // Remove all entries from the OS 50k gazetteer table.
        void clear_gazetteer(sqlite::connection&);

        /*
         * Remove all entries from the OS 50k gazetteer table where the entry
         * is inside one of the given regions.
         */
        void clear_gazetteer(
                const std::vector<std::string>&,
                sqlite::connection&
                );

        void delete_tile_data(
                const std::string& region,
                int eastings,
                int northings,
                sqlite::connection& conn
                );

        void search_gazetteer(
                sqlite::connection& conn,
                const std::string& search_term,
                json::list&
                );
    }
}

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::map::tile,
        (std::string&, region())
        (int&, eastings())
        (int&, northings())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::map::tile_data,
        (std::string&, region())
        (int&, eastings())
        (int&, northings())
        (std::string&, data())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::map::tile_data_db,
        (std::string, region)
        (int, eastings)
        (int, northings)
        (std::vector<unsigned char>, data)
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::map::gazetteer_record,
        (int&, seq())
        (std::string&, km_ref())
        (std::string&, def_nam())
        (std::string&, tile_ref())
        (int&, lat_deg())
        (double&, lat_min())
        (int&, long_deg())
        (double&, long_min())
        (int&, north())
        (int&, east())
        (std::string&, gmt())
        (std::string&, co_code())
        (std::string&, county())
        (std::string&, full_county())
        (std::string&, f_code())
        (std::string&, e_date())
        (std::string&, update_co())
        (int&, sheet_1())
        (int&, sheet_2())
        (int&, sheet_3())
        )

#endif

