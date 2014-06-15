/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jpeg_data.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/fusion/include/vector.hpp>

#include <exiv2/exiv2.hpp>
#include <Magick++.h>

#include "base64/base64.hpp"
#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

#include "photograph_db.hpp"

void photograph::api::insert_jpeg_data(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    db::insert(::photograph::jpeg_data(request.params()[0]), db);

    // Decode the jpeg data to get the date taken
    std::string jpeg_bin = base64::decode(
            ::photograph::jpeg_data(request.params()[0]).data()
            );

    try
    {
        auto image = Exiv2::ImageFactory::open(
                reinterpret_cast<const unsigned char*>(jpeg_bin.c_str()),
                jpeg_bin.length()
                );
        image->readMetadata();
        Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");

        // Try to find the date key
        Exiv2::ExifData::iterator pos = image->exifData().findKey(
                Exiv2::ExifKey("Exif.Image.DateTimeOriginal")
                );
        if( pos == image->exifData().end() )
            pos = image->exifData().findKey(
                    Exiv2::ExifKey("Exif.Image.DateTime")
                    );

        // If an acceptable key was found
        if( pos != image->exifData().end() )
        {
            std::string date = pos->getValue()->toString();

            // Exif date format
            auto *df = new boost::date_time::time_input_facet<
                boost::posix_time::ptime,
                char
                >("%Y:%m:%d %H:%M:%S");

            std::stringstream date_stream(date);
            date_stream.imbue(std::locale(date_stream.getloc(), df));
            boost::posix_time::ptime taken;

            try
            {
                date_stream >> taken;

                json::object o = json::map();
                photograph photo(o);
                db::get_by_id(
                        ::photograph::jpeg_data(request.params()[0]).id(),
                        db,
                        photo
                        );

                // Update the photograph with the date taken data
                photo.taken() =
                    boost::posix_time::to_iso_extended_string(taken);
                db::update(photo, db);
            }
            catch(const std::exception&) {}
        }
        else
        {
            result.error() = "Could not find date in photograph.";
        }
    }
    catch(const std::exception& e)
    {
        result.error() = "Inserting JPEG data.";
    }
}

void photograph::api::jpeg_data(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    int id = request.params().get<int>(0);
    json::object o = json::map();
    ::photograph::jpeg_data data(o);
    db::get_by_id(id, db, data);
    result.data() = o;
}

void photograph::api::jpeg_data_scaled(
        jsonrpc::request&   request,
        jsonrpc::result&    result,
        sqlite::connection& db
        )
{
    int id = request.params().get<int>(0);
    json::object o = json::map();
    ::photograph::jpeg_data data(o);
    db::get_by_id(id, db, data);

    // Load the image
    std::string decoded = base64::decode(data.data());

    Magick::Image image(Magick::Blob(
        reinterpret_cast<const void*>(decoded.c_str()), decoded.length())
        );

    short orientation = 1;

    try
    {   // Retrieve orientation
        auto exiv_image = Exiv2::ImageFactory::open(
            reinterpret_cast<const unsigned char*>(decoded.c_str()),
            decoded.length()
            );
        exiv_image->readMetadata();

        Exiv2::ExifKey key("Exif.Image.Orientation");
        Exiv2::ExifData::iterator pos = exiv_image->exifData().findKey(key);

        if( pos != exiv_image->exifData().end() )
            orientation = pos->getValue()->toLong();
    }
    catch(const std::exception&)
    {
    }

    try
    {   // Scale the image
        int width = request.params().get<int>(1);
        int height = request.params().get<int>(2);
        width = width?width:100;
        height = height?height:100;
        std::ostringstream oss;
        switch(orientation)
        {
        case 6:
        case 8:
            // Swap width and height because the image is about to be rotated
            std::swap(width, height);
        }
        // Scale to fit within this box
        oss << width << "x" << height << "^";
        image.scale(Magick::Geometry(oss.str()));
    }
    catch(const std::exception& e)
    {
    }

    try
    {
        // Rotate the image
        switch( orientation )
        {
            case 3:
                image.rotate(180);
            case 6:
                image.rotate(90);
            case 8:
                image.rotate(270);
        }
    }
    catch(const std::exception&)
    {
    }

    try
    {
        // Write the image in base64
        Magick::Image out_image( image.size(), Magick::Color(255,255,255) );
        out_image.composite(image, 0, 0);
        Magick::Blob blob;
        out_image.write(&blob, "JPEG");
        data.data() = base64::encode(
            reinterpret_cast<const unsigned char*>(blob.data()),
            blob.length()
            //reinterpret_cast<const unsigned char*>(blob.data() + blob.length())
            );
    }
    catch(const std::exception& e)
    {
    }

    result.data() = o;
}

