/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "exports/exports.hpp"

#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "commandline/commandline.hpp"
#include "sqlite/connection.hpp"
#include "sqlite/row.hpp"
#include "sqlite/select.hpp"

#include "db/jpeg_data.hpp"
#include "util/scale.hpp"

int photograph::exports::main(int argc, const char* argv[])
{
    bool show_help = false, fullsize = false;
    std::string db_file, output_directory, geometry;
    std::vector<commandline::option> options{
        commandline::parameter("db", db_file, "Database file path"),
        commandline::parameter("out", output_directory, "Output directory"),
        commandline::parameter("geometry", geometry, "ImageMagick geometry"),
        commandline::flag("fullsize", fullsize, "Do not scale images"),
        commandline::flag("help", show_help, "Show a help message")
    };
    commandline::parse(argc, argv, options);

    if(show_help)
    {
        commandline::print(argc, argv, options);
        return 0;
    }

    if(db_file.empty())
    {
        std::cerr << "database file not specified" << std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }

    if(output_directory.empty())
    {
        std::cerr << "output directory not specified" << std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }

    if((!fullsize && geometry.empty()) || (fullsize && geometry.length()))
    {
        std::cerr <<
            "exactly one of --fullsize and --geometry must be specified" <<
            std::endl;
        commandline::print(argc, argv, options);
        return 1;
    }

    sqlite::connection conn(db_file);
    boost::filesystem::path out_dir(output_directory);

    sqlite::rowset<int, std::string> albums;
    sqlite::select(
            conn,
            "SELECT album_id, name FROM album ",
            sqlite::empty_row(),
            albums
            );
    for(const sqlite::row<int, std::string> album : albums)
    {
        std::cerr << "exporting album " << sqlite::column<0>(album) << std::endl;
        boost::filesystem::path album_dir = out_dir/sqlite::column<1>(album);
        boost::filesystem::create_directory(album_dir);
        sqlite::rowset<std::string, std::vector<unsigned char>> photographs;
        sqlite::select(
                conn,
                "SELECT taken, data "
                "FROM album NATURAL JOIN photograph NATURAL JOIN jpeg_data "
                "WHERE album_id = ? "
                "ORDER BY taken ASC ",
                sqlite::row<int>(sqlite::column<0>(album)),
                photographs
                );

        static const char *unknown_string = "unknown";
        std::string last_taken = unknown_string;
        int count = 0;
        for(const sqlite::row<std::string, std::vector<unsigned char>> p : photographs)
        {
            // Write out the photograph.
            const std::string taken = sqlite::column<0>(p).length()?
                    std::string(sqlite::column<0>(p), 0, 10):unknown_string;
            if(taken == last_taken)
                count++;
            else
            {
                count = 1;
                last_taken = taken;
            }

            std::ostringstream out_filename;
            out_filename << taken << "_" << std::setfill('0') <<
                std::setw(4) << count << ".jpg";
            std::cerr << "export photograph " << out_filename.str() << std::endl;

            boost::filesystem::ofstream os(album_dir/out_filename.str());
            if(fullsize)
            {
                const std::vector<unsigned char>& data = sqlite::column<1>(p);
                os << std::string((const char*)(&(data[0])), data.size());
            }
            else
            {
                std::vector<unsigned char> data;
                util::scale(
                        sqlite::column<1>(p),
                        geometry,
                        data
                        );
                os << std::string((const char*)(&(data[0])), data.size());
            }
        }
    }
}

int main(int argc, const char* argv[])
{
    photograph::exports::main(argc, argv);
}

