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
        boost::filesystem::path album_dir = out_dir/sqlite::column<1>(album);
        std::cerr << "exporting to directory " << album_dir << std::endl;
        boost::filesystem::create_directory(album_dir);

        sqlite::rowset<int> photographs;
        sqlite::select(
                conn,
                "SELECT photograph.photograph_id "
                "FROM album JOIN photograph_in_album "
                "ON album.album_id = photograph_in_album.album_id "
                "JOIN photograph "
                "ON photograph_in_album.photograph_id = photograph.photograph_id "
                "WHERE album.album_id = ? "
                "ORDER BY taken ASC ",
                sqlite::row<int>(sqlite::column<0>(album)),
                photographs
                );

        static const char *unknown_string = "unknown";
        std::string last_taken = unknown_string;
        int count = 0;
        for(const sqlite::row<int> photo_id : photographs)
        {
            sqlite::row<std::string, std::vector<unsigned char>> photo_data;
            {
                sqlite::rowset<std::string, std::vector<unsigned char>> data_list;
                sqlite::select(
                    conn,
                    "SELECT taken, data "
                    "FROM photograph NATURAL JOIN jpeg_data "
                    "WHERE photograph_id = ? ",
                    photo_id,
                    data_list
                    );
                if(!data_list.size())
                {
                    std::cerr << "no photograph with id " << sqlite::column<0>(photo_id) << std::endl;
                    continue;
                }
                photo_data = std::move(data_list[0]);
            }

            // Write out the photograph.
            const std::string taken = sqlite::column<0>(photo_data).length()?
                    std::string(sqlite::column<0>(photo_data), 0, 10):unknown_string;
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

            const boost::filesystem::path jpeg_file = album_dir/out_filename.str();
            std::cerr << "export photograph to " << jpeg_file << std::endl;
            boost::filesystem::ofstream os(jpeg_file);

            if(fullsize)
            {
                const std::vector<unsigned char>& data = sqlite::column<1>(photo_data);
                os << std::string((const char*)(&(data[0])), data.size());
            }
            else
            {
                std::vector<unsigned char> data;
                util::scale(
                        sqlite::column<1>(photo_data),
                        geometry,
                        data
                        );
                os << std::string((const char*)(&(data[0])), data.size());
            }
            os.close();
        }
    }
}

int main(int argc, const char* argv[])
{
    photograph::exports::main(argc, argv);
}

