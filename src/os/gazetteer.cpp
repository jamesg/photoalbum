/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "gazetteer.hpp"

/*
 * gazetteer.cpp - utility to populate the gazetteer table in a SQLite maps
 * database.
 */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/tokenizer.hpp>

#include "commandline/commandline.hpp"

#include "json/json.hpp"

#include "sqlite/connection.hpp"
#include "sqlite/insert.hpp"

#include "db/map.hpp"
#include "fill_container.hpp"

int photograph::os::gazetteer(int argc, const char *argv[])
{
    bool show_help = false;
    std::string data_file, mapdb;
    std::vector<std::string> regions;

    std::vector<commandline::option> options{
        commandline::parameter("data", data_file, "Gazetteer data file"),
        commandline::flag("help", show_help, "Show this help message"),
        commandline::parameter("mapdb", mapdb, "Map SQLite database"),
        commandline::list("regions", regions, "Regions to replace")
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

    std::ifstream in(data_file);
    if(!in.is_open())
    {
        std::cerr << "Error opening " << data_file << " for reading." << std::endl;
        return 1;
    }

    sqlite::connection conn(mapdb);
    db::map::create(conn);

    // Ready to start importing gazetteer data.
    // Assume we are replacing regions on the command line.
    if(regions.empty())
        db::clear_gazetteer(conn);
    else
        db::clear_gazetteer(regions, conn);

    json::list records;
    std::string line;
    while(std::getline(in, line))
    {
        // The data set may have DOS line endings.
        if(line.size() && line.at(line.size()-1) == '\r')
            line.resize(line.size()-1);

        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
        tokenizer t(line, boost::escaped_list_separator<char>("\\", ":", "\""));
        std::vector<json::object> values;
        std::copy(t.begin(), t.end(), std::back_inserter(values));

        if(values.size() != 20)
        {
            std::cerr << "warning: wrong number of values in gazetteer row " <<
                values.size() << std::endl;
            continue;
        }

        json::object obj;
        map::gazetteer_record record(obj);
        fill_container(values, record);

        // Insert the record only if it is in a region of interest.
        std::string code(record.km_ref(), 0, 2);
        if(
            regions.size() &&
            std::find(regions.begin(), regions.end(), code) == regions.end()
            )
            continue;

        //db::insert(record, conn);
        records.append(obj);
    }

    db::insert_gazetteer_records(records, conn);

    std::cerr << "inserted " << records.size() << "records" << std::endl;

    return 0;
}

int main(int argc, const char* argv[])
{
    photograph::os::gazetteer(argc, argv);
}

