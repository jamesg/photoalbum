/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "server.hpp"

#include <iostream>

#include "jsonrpc/request.hpp"
#include "jsonrpc/result.hpp"

jsonrpc::server::server()
{
}

void jsonrpc::server::install(
        const std::string& name,
        method_type method
        )
{
    m_methods[name] = detail::method{method, [](...) { return true; }};
}

void jsonrpc::server::install(
        const std::string& name,
        method_type method,
        boost::function<bool(const request&)> auth_function
        )
{
    m_methods[name] = detail::method{method, auth_function};
}

void jsonrpc::server::serve(request& req, result& res) const
{
    auto method_it = m_methods.find(req.method());
    if(method_it == m_methods.end())
    {
        std::cerr << "jsonrpc request for unknown method: " << req.method() <<
            std::endl;
        res.error() = "Method unknown";
        return;
    }

    try
    {
        const bool authed = method_it->second.check_auth(req);
        if(!authed)
        {
            res.error() = "Not authorised";
            return;
        }
    }
    catch(const std::exception& e)
    {
        res.error() = "Checking authentication";
        std::cerr << "checking authentication: " << e.what() << std::endl;
        return;
    }

    try
    {
        method_it->second.serve(req, res);
    }
    catch(const std::exception& e)
    {
        if(res.error().empty())
            res.error() = "Unknown error";
        std::cerr << "error in api function " << req.method() << ": " <<
            e.what() << std::endl;
    }
}

