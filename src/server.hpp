#ifndef PHOTOGRAPH_SERVER_HPP
#define PHOTOGRAPH_SERVER_HPP

/*
 * Photoalbum - a photograph album web application.
 * Copyright (C) 2014 James Goode.
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility.hpp>

#include "mongoose.h"

#include "jsonrpc/method_type.hpp"
#include "uri/uri_type.hpp"

namespace photograph
{
    class server : boost::noncopyable
    {
    public:
        server(
                const std::string& document_root,
                const std::string& port
                );
        ~server();
        void listen();
        void install(const std::string& uri, uri::uri_type function);
        const boost::posix_time::ptime& start_time() const;
    private:
        static int accept(mg_connection*, mg_event);
        int serve(mg_connection*, mg_event) const;
        mg_server *m_server;
        std::map<std::string, jsonrpc::method_type> m_api_methods;
        std::map<std::string, uri::uri_type> m_uri;
        boost::posix_time::ptime m_start_time;
    };

    server& get_server();
}

#endif

