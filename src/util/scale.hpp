#ifndef PHOTOGRAPH_UTIL_SCALE_HPP
#define PHOTOGRAPH_UTIL_SCALE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>
#include <vector>

namespace photograph
{
    namespace util
    {
        /*
         * Scale an image.
         *
         * @geometry An ImageMagick geometry string.
         */
        void scale(
                const std::vector<unsigned char> image,
                const std::string& geometry,
                std::vector<unsigned char>& scaled_image
                );

        /*
         * Scale an image.
         *
         * @width Width of the resulting image.
         * @height Height of the resulting image.
         */
        void scale(
                const std::vector<unsigned char> image,
                const int width,
                const int height,
                std::vector<unsigned char>& scaled_image
                );
    }
}

#endif

