#ifndef BAS64_BASE64_HPP
#define BAS64_BASE64_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <string>

namespace base64
{
    std::string encode(unsigned char const* , unsigned int len);
    std::string decode(std::string const& s);
}

#endif

