Photograph Album
================

A photograph album web application.  The back end server is written in C++ and
uses SQLite for data storage and libmongoose as the web server.

Modules
--------

* Photograph: Upload and store photographs, categorise photographs into albums and write textual notes.
* Map: Search for map landmarks and view Ordance Survey Streetmap tiles.
* Auth: Restrict access to the software.
* Cache: Speed up access to photograph thumbnails.

Dependencies
------------

C++ server:

* libsqlite3
* libexiv2
* libmagick++
* Boost

Web client:

* Node JS and NPM (included with recent Node).

Building
--------

1. Clone the Git repository.  Run git submodule init; git submodule update to
   download the required command line, JSON and SQLite libraries.
2. Create a build directory (this could be anywhere, build/ inside the
   repository root is fine).
3. Run cmake in the build directory, passing the repository root as the first
   argument (for instance, "cmake .." fi the build directory is directly inside
   the repository root).
4. Build the project using your make tool (probably make).

### Targets

These targets can be built individually:

* photoalbum - The main server.
* bundle - Web application Javascript bundle.
* exports - Utility to export photographs as JPEG files.
* gazetteer - Utility to import Ordnance Survey Gazetteer data.
* streetmap - Utility to import Ordnance Survey Street Map tile images.

Installation
------------

Database files specified as command line parameters will be initialised when the
program is started.  Different databases may be specified for each module
(photograph, map, cache and auth) or the same database can be used for some or
all modules.

Databases for the photograph and map modules must be specified for their
modules to be used.  If no database is specified for the auth or cache module
an in-memory database will be used instead.

It is a good idea to keep at least the map and cache databases separate from
the photograph and auth databases because the map and cache databases can be
generated from the Ordnance Survey data and the photograph database, while the
photograph and auth databases cannot be regenerated and are most likely to be
included in backups.

Usage
-------

    bin/main --db photograph_database_name.db --document-root web/static

### Command Line Options

* db: photograph database file
* authdb: authentication database file
* document-root: location of static files to serve at the web server root

Run bin/main --help for a complete command line options reference.

Design
------

The application is built in two parts: a C++ web server and a Javascript web
application.  These components communicate over a JSONRPC API.

### Server Side

Libmongoose is used on the server side for handling HTTP requests.  The JSONRPC
API is provided by a custom library (look at src/jsonrpc/server.hpp).

Code is split into modules.  Each module has a source and header (for large
modules, a directory) in each of src/api and src/db where API functions and
database functions for the module are kept.  Although there are some
inter-dependencies between modules, they operate mostly independently, having
their own API functions, database storage and user interfaces.

#### Namespacing

A standard namespacing scheme is used across all C++ code to define boundaries
between modules.  The project`s root namespace is ::photoalbum.  A module may
have database functions, JSONRPC API functions and URI functions.  These are
placed in ::photograph::[module]::db, ::photograph::[module]::api and
::photograph::[uri]:: respectively.  The exception to this rule are heavily
overloaded database functions such as get_by_id(id, conn, out) and update(in,
conn).  These functions are located in ::photograph::db.

### Web Application

The client is a single page web application.  It is compiled from sources in
web/client using Webmake and is delivered to the client as one HTML and one
Javascript file.

TODO List
---------

* Maps module.
  The maps module currently imports data from the Ordnance Survey 50k Gazetteer
  and Stree Map data sets, provides searching by landmark name and displays
  individual map tiles.  The next steps are locating photographs on the map by
  storing a grid reference in the photograph database and displaying photograph
  locations on the map tiles.
* Smaller issues:
    * Remove old images from the cache.

