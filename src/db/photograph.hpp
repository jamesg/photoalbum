#ifndef DB_PHOTOGRAPH_HPP
#define DB_PHOTOGRAPH_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "json/json.hpp"

#include "has_id.hpp"

namespace photoalbum
{
    extern const char photograph_id_cstr[];
    /*!
     * Photograph metadata.
     */
    struct photograph : public has_id<photograph_id_cstr>
    {
        photograph(json::object& m) :
            has_id(m)
        {
        }
        std::string& title() const { return get_string("title"); }
        std::string& caption() const { return get_string("caption"); }
        std::string& filename() const { return get_string("filename"); }
        std::string& location() const { return get_string("location"); }
        std::string& taken() const { return get_string("taken"); }
        /*!
         * \brief List of tags (strings).
         */
        json::list& tags() const { return get_list("tags"); }
    };

    /*!
     * Records the fact that a photograph id is in an album id.
     */
    struct photograph_in_album : public json::map_accessor
    {
        photograph_in_album(json::object& m) :
            json::map_accessor(m)
        {
        }
        int& photograph_id() const { return get_int("photograph_id"); }
        int& album_id() const { return get_int("album_id"); }
    };

    /*!
     * Records the fact that a photograph id has been tagged with a string
     * tag.
     */
    struct photograph_tagged : public json::map_accessor
    {
        photograph_tagged(json::object& m) :
            json::map_accessor(m)
        {
        }
        int& photograph_id() const { return get_int("photograph_id"); }
        std::string& tag() const { return get_string("tag"); }
    };
}

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace db
    {
        namespace photograph
        {
            void create(sqlite::connection& photograph_db);
        }

        int insert(const photoalbum::photograph&, sqlite::connection& db);
        void update(const photoalbum::photograph&, sqlite::connection& db);
        void get_by_id(
                int                           id,
                sqlite::connection&           photograph_db,
                const photoalbum::photograph& photo
                );
        void get_photograph_list(sqlite::connection& db, json::list& list);
        int insert(const photograph_in_album&, sqlite::connection& db);
        void get_photographs_by_album(
                sqlite::connection& db,
                const int album_id,
                json::list& list
                );
    }
}

#endif

