/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "streetmap.hpp"

/*
 * streetmap.cpp - import Ordnance Survey street map graphics from TIFF files
 * to a SQLite database.
 */

#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <Magick++.h>

#include "commandline/commandline.hpp"
#include "sqlite/connection.hpp"

#include "db/map.hpp"

int photograph::os::streetmap(int argc, const char* argv[])
{
    bool show_help = false;
    std::string mapdb;
    std::vector<std::string> file_list;

    std::vector<commandline::option> options{
        commandline::list("files", file_list, "TIFF files to import"),
        commandline::flag("help", show_help, "Show this help message"),
        commandline::parameter("mapdb", mapdb, "Map SQLite database")
    };

    commandline::parse(argc, argv, options);

    if(show_help)
    {
        commandline::print(argc, argv, options);
        return 0;
    }

    if(mapdb.empty())
    {
        std::cerr << "error: database not specified" << std::endl;
        return 1;
    }

    sqlite::connection conn(mapdb);
    db::map::create(conn);

    if(file_list.empty())
        std::cerr << "warning: no files specified" << std::endl;

    for(const std::string& filename : file_list)
    {
        std::cerr << "inserting image " << filename << std::endl;

        try
        {
            Magick::Image image(filename);
            Magick::Geometry out_size(
                image.size().width()/5,
                image.size().height()/5
                );

            boost::filesystem::path path(filename);
            // Region from the filename (also the region for the result).
            const std::string region  =
                std::string(path.filename().string(), 0, 2);
            // Quarter used to derive eastings and northings.
            const std::string quarter =
                std::string(path.filename().string(), 4, 2);
            // Eastings and northings from the filename.  Convert the
            // single-digit (10km) references to two-digit (1km) references.
            const int eastings_10k = boost::lexical_cast<int>(
                    std::string(path.filename().string(), 2, 1)
                    );
            const int northings_10k = boost::lexical_cast<int>(
                    std::string(path.filename().string(), 3, 1)
                    );
            const int eastings =
                eastings_10k*10 + ((quarter.at(1)=='e')?5:0);
            const int northings =
                northings_10k*10 + ((quarter.at(0)=='n')?5:0);

            // Each quarter tile contains 5x5 kilometre tiles.
            for(int ix=0; ix<5; ++ix)
            {
                for(int iy=0; iy<5; ++iy)
                {
                    try
                    {
                        map::tile_data_db data;
                        Magick::Image out_image(out_size, Magick::Color(255,255,255));
                        Magick::Blob out_blob;

                        int off_x = -(out_size.width() * ix);
                        int off_y = -(out_size.height() * (4 - iy));
                        out_image.composite(image, off_x, off_y);

                        out_image.write(&out_blob, "PNG");

                        data.region    = region;
                        data.eastings  = eastings + ix;
                        data.northings = northings + iy;
                        const unsigned char* blob_data =
                                reinterpret_cast<const unsigned char*>(out_blob.data());
                        for(int i = 0; i < out_blob.length(); ++i)
                            data.data.push_back(blob_data[i]);

                        db::delete_tile_data(
                                data.region,
                                data.eastings,
                                data.northings,
                                conn
                                );
                        db::insert(data, conn);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "error inserting image: " <<
                            region << eastings << northings << " " <<
                            e.what() << std::endl;
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "error inserting image file " << filename << ": " << e.what() << std::endl;
        }
    }
}

int main(int argc, const char* argv[])
{
    photograph::os::streetmap(argc, argv);
}

