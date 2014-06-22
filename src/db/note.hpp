#ifndef DB_NOTE_HPP
#define DB_NOTE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/fusion/include/adapt_struct.hpp>

#include "has_id.hpp"

namespace photoalbum
{
    extern const char markdown_id_str[];
    extern const char note_id_str[];
    extern const char note_version_id_str[];

    struct note :
        public has_id<note_id_str>
    {
        note(json::object& m) :
            has_id(m)
        {
        }
        std::string& title()    const { return get_string("title"); }
        // Date the note was created.
        std::string& created()  const { return get_string("created"); }
    };

    /*
     * Version of a note in a particular phase (draft, published).  There can
     * only be one version of each note for each phase.
     */
    struct note_version :
        public has_id<note_version_id_str>
    {
        enum phase_t
        {
            draft     = 0,
            published = 1
        };

        note_version(json::object& m) :
            has_id(m)
        {
        }
        // Note id references note.
        int& note_id()          const { return get_int(note_id_str); }
        // Markdown id references markdown.
        int& markdown_id()      const { return get_int(markdown_id_str); }
        // Member of enum phase_t.
        int& phase()            const { return get_int("phase"); }
        // Date this version was last modified.
        std::string& modified() const { return get_string("modified"); }
    };

    /*
     * Storage for markdown data.
     */
    struct markdown_data :
        public has_id<markdown_id_str>
    {
        markdown_data(json::object& m) :
            has_id(m)
        {
        }
        std::string& markdown() const { return get_string("markdown"); }
    };

    /*
     * All data required to display a publically viewable note (phase ==
     * published).
     */
    struct published_note :
        public has_id<note_id_str>
    {
        published_note(json::object& m) :
            has_id(m)
        {
        }
        std::string& created() const { return get_string("created"); }
        std::string& modified() const { return get_string("modified"); }
        std::string& markdown() const { return get_string("markdown"); }
    };

    /*
     * A photograph is available for use in a note.  It can be referenced in
     * the markdown with the filename.
     */
    struct note_version_uses_photograph :
        public json::map_accessor
    {
        note_version_uses_photograph(json::object& m) :
            map_accessor(m)
        {
        }
        int&         note_id()       const { return get_int("note_id"); }
        int&         photograph_id() const { return get_int("photograph_id"); }
        std::string& filename()      const { return get_string("filename"); }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::note,
        (int&,         id())
        (std::string&, title())
        (std::string&, created())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::note_version,
        (int&,         id())
        (int&,         note_id())
        (int&,         markdown_id())
        (int&,         phase())
        (std::string&, modified())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::markdown_data,
        (int&,         id())
        (std::string&, markdown())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::published_note,
        (std::string&, created())
        (std::string&, modified())
        (std::string&, markdown())
        )

BOOST_FUSION_ADAPT_STRUCT(
        ::photoalbum::note_version_uses_photograph,
        (int&,         note_version_id())
        (int&,         photograph_id())
        (std::string&, filename())
        )

namespace sqlite
{
    class connection;
}

namespace photoalbum
{
    namespace db
    {
        int insert(const note&, sqlite::connection&);
        void update(const note&, sqlite::connection&);
        int insert(const markdown_data&, sqlite::connection&);
        void update(const markdown_data&, sqlite::connection&);
        int insert(const note_version&, sqlite::connection&);
        void delete_note(int, sqlite::connection&);
        void delete_note_version(int, note_version::phase_t, sqlite::connection&);
        void get(int, sqlite::connection&, const note&);
        void get_note_list(sqlite::connection&, json::list&);
        void get_published_notes(sqlite::connection&, json::list&);
        void get(
                int note_id,
                note_version::phase_t,
                sqlite::connection&,
                const markdown_data&
                );
        void get(
                int markdown_id,
                sqlite::connection&,
                const markdown_data&
                );
        void get(
                int note_id,
                note_version::phase_t,
                sqlite::connection&,
                const note_version&
                );
        void get(
                int note_version_id,
                sqlite::connection&,
                const note_version&
                );
        void get(int note_id, sqlite::connection&, const published_note&);
    }
}

#endif

