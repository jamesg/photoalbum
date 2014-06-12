/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include "server.hpp"

#include <iostream>

#include <boost/bind.hpp>

#include "mongoose.h"

photograph::server::server(
        const std::string& document_root,
        const std::string& port
        ) :
    m_server(nullptr)
{
    m_server = mg_create_server((void*)this, &server::accept);
    mg_set_option(m_server, "document_root", document_root.c_str());
    mg_set_option(m_server, "listening_port", port.c_str());
}

photograph::server::~server()
{
    mg_destroy_server(&m_server);
}

void photograph::server::listen()
{
    m_start_time = boost::posix_time::second_clock::universal_time();
    for(;;)
        mg_poll_server(m_server, 1000);
}

void photograph::server::install(const std::string& uri, uri::uri_type function)
{
    m_uri[uri] = function;
}

const boost::posix_time::ptime& photograph::server::start_time() const
{
    return m_start_time;
}

int photograph::server::accept(mg_connection *conn, mg_event ev)
{
    return ((server*)(conn->server_param))->serve(conn, ev);
}

int photograph::server::serve(mg_connection *conn, mg_event ev) const
{
    if(ev == MG_AUTH)
        return MG_TRUE;
    if(!conn->uri)
        return MG_FALSE;
    auto uri_it = m_uri.find(std::string(conn->uri));
    if(uri_it == m_uri.end())
    {
        return MG_FALSE;
    }
    else
    {
        try
        {
            return uri_it->second(conn, ev);
        }
        catch(const std::exception& e)
        {
            std::cerr << "error serving URI " << conn->uri << ": " << e.what() <<
                std::endl;
            return MG_FALSE;
        }
    }
}

