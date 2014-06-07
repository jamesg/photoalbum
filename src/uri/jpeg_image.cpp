/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "jpeg_image.hpp"

#include <boost/lexical_cast.hpp>

#include <exiv2/exiv2.hpp>
#include <Magick++.h>

#include "db/get_database.hpp"
#include "db/jpeg_data.hpp"
#include "server.hpp"
#include "uri/http_date.hpp"

int photograph::uri::jpeg_image(
        const server& serve,
        mg_connection *conn,
        mg_event ev
        )
{
    if(ev != MG_REQUEST)
        return MG_FALSE;
    char id_s[10], width_s[10], height_s[10];
    mg_get_var(conn, "photograph_id", id_s, sizeof(id_s));
    mg_get_var(conn, "width", width_s, sizeof(width_s));
    mg_get_var(conn, "height", height_s, sizeof(height_s));

    int width = 100, height = 100;
    try
    {
        width = boost::lexical_cast<int>(width_s);
    }
    catch(const std::exception&)
    {
    }
    try
    {
        height = boost::lexical_cast<int>(height_s);
    }
    catch(const std::exception&)
    {
    }
    try
    {
        int photo_id = boost::lexical_cast<int>(id_s);
        json::object data_o;
        jpeg_data_db data(data_o);
        db::get_by_id(
                photo_id,
                db::get_database(db::database_photograph),
                data
                );
        Magick::Image image(Magick::Blob(
            reinterpret_cast<const void*>(&(data.data[0])), data.data.size())
            );
        short orientation = 1;

        try
        {   // Retrieve orientation
            auto exiv_image = Exiv2::ImageFactory::open(
                reinterpret_cast<const unsigned char*>(&(data.data[0])),
                data.data.size()
                );
            exiv_image->readMetadata();

            Exiv2::ExifKey key("Exif.Image.Orientation");
            Exiv2::ExifData::iterator pos = exiv_image->exifData().findKey(key);

            if( pos != exiv_image->exifData().end() )
                orientation = pos->getValue()->toLong();
        }
        catch(const std::exception&)
        {
            // Some images don't have an orientation.
        }

        try
        {
            // Scale the image
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
            Magick::Image out_image( image.size(), Magick::Color(255,255,255) );
            out_image.composite(image, 0, 0);
            Magick::Blob blob;
            out_image.write(&blob, "JPEG");
            mg_send_status(conn, 200);
            mg_send_header(conn, "Content-type", "image/jpeg");
            mg_send_header(conn, "Last-Modified", http_date(serve.start_time()).c_str());
            mg_send_data(conn, blob.data(), blob.length());
        }
        catch(const std::exception& e)
        {
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "in jpeg_image: " << e.what() << std::endl;
        return MG_FALSE;
    }

    return MG_TRUE;
}

