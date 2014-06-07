#ifndef JSONRPC_METHOD_TYPE_HPP
#define JSONRPC_METHOD_TYPE_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <functional>

#include "result.hpp"
#include "request.hpp"
#include "sqlite/connection.hpp"

namespace jsonrpc
{
    typedef std::function<void(request&, result&)> method_type;
}

#endif

