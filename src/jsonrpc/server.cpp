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
    m_methods[name] = method;
}

void jsonrpc::server::serve(request& req, result& res) const
{
    try
    {
        auto method_it = m_methods.find(req.method());
        if(method_it == m_methods.end())
            res.error() = "Method unknown";
        else
            method_it->second(req, res);
    }
    catch(const std::exception& e)
    {
        res.error() = "Unknown error";
        std::cerr << "error in api function " << req.method() << ": " <<
            e.what() << std::endl;
    }
}

