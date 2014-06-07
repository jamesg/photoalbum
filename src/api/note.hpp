#ifndef API_NOTE_HPP
#define API_NOTE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

namespace jsonrpc
{
    struct request;
    struct result;
}

namespace sqlite
{
    class connection;
}

namespace photograph
{
    namespace api
    {
        void markdown_data(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        // Get all notes (including drafts).
        void note_list(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        // Get a note version by id.
        void note_version(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        // Get a note version by note id and phase.
        void note_phase_version(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void insert_note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void create_draft_note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void delete_note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        // Get published notes (only those with a published note_version).
        void published_notes(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void update_markdown_data(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        // Copy the note's draft markdown data to published markdown data.
        void publish_note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
        void published_note(
                jsonrpc::request&,
                jsonrpc::result&,
                sqlite::connection&
                );
    }
}

#endif

