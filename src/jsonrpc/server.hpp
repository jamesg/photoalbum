#ifndef PHOTOGRAPH_JSONRPC_SERVER_HPP
#define PHOTOGRAPH_JSONRPC_SERVER_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <map>

#include <boost/function.hpp>

#include "jsonrpc/method_type.hpp"

namespace jsonrpc
{
    struct request;
    struct result;

    namespace detail
    {
        struct method
        {
            method_type serve;
            boost::function<bool(const request&)> check_auth;
        };
    }

    class server
    {
    public:
        server();
        void install(const std::string&, method_type);
        void install(
            const std::string&,
            method_type,
            boost::function<bool(const request&)>
            );
        void serve(request&, result&) const;
    private:
        std::map<std::string, detail::method> m_methods;
    };
}

#endif

