Photograph Album
================

A photograph album web application.  The back end server is written in C++ and
uses SQLite for data storage and libmongoose as the web server.

Features
--------

* Upload JPEG photographs.
* Assign photographs to albums.  A photograph can be a member of zero, one or
  more albums.
* Assign tags to photographs.
* Browse photographs by album, tag or location.
* Write text notes in Markdown.

Dependencies
------------

C++ server:

* libsqlite3
* libexiv2
* libmagick++
* Boost

Web client:

* Node JS and NPM (included with recent Node)

Building
--------

1. Clone the Git repository.  Run git submodule init; git submodule update to
   download the required command line, JSON and SQLite libraries.
2. Run make.
3. Change to web/ and run npm install; make to build the web client.

Installation
------------

A SQLite database must be populated from a setup script.

    sqlite3 database_name.db
    sqlite> .read install.sql

Usage
-------

    bin/main --db database_name.db --document-root web/static

Run bin/main --help for command line options reference.

Design
------

The application is built around a libmongoose web server.  There are handlers
for serving static content, JPEG images from the database and JSONRPC API
requests.  URI functions are located in the uri directory.

All data that can be encoded as JSON is accessed and updated through the
JSONRPC API.  API functions are located in the api directory.

The client is a single page web application.  It is compiled from sources in
web/client using Webmake and is delivered to the client as a single Javascript
file.

Maps
----

Work is ongoing to implement linking photographs to locations.

