#ifndef PHOTOGRAPH_JSONRPC_SERVER_HPP
#define PHOTOGRAPH_JSONRPC_SERVER_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <map>

#include "jsonrpc/method_type.hpp"

namespace jsonrpc
{
    struct request;
    struct result;

    class server
    {
    public:
        server();
        void install(const std::string&, method_type);
        void serve(request&, result&) const;
    private:
        std::map<std::string, method_type> m_methods;
    };
}

#endif

