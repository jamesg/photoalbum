cmake_minimum_required(VERSION 2.6)

# Submodule libraries
set(SQLITE_JSON_LIBRARY_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(json)
add_subdirectory(sqlite)

add_subdirectory(web)

project(PhotoAlbum)

set(CMAKE_CXX_FLAGS "-std=c++0x")

# Executable sources
file(GLOB exports_sources    src/exports/*.cpp)
file(GLOB gazetteer_sources  src/os/gazetteer.cpp)
file(GLOB photoalbum_sources src/server.cpp src/photoalbum/*.cpp)
file(GLOB streetmap_sources  src/os/streetmap.cpp)

# Library sources
file(GLOB api_sources      src/api/*.cpp src/api/*/*.cpp)
file(GLOB base64_sources   src/base64/*.cpp)
file(GLOB db_sources       src/db/*.cpp src/db/*/*.cpp)
file(GLOB jsonrpc_sources  src/jsonrpc/*.cpp)
file(GLOB mongoose_sources src/mongoose.c)
file(GLOB uri_sources      src/uri/*.cpp)
file(GLOB util_sources     src/util/*.cpp)

# External libraries
find_package(Threads)
find_package(Boost COMPONENTS date_time filesystem random regex system)
find_package(ImageMagick COMPONENTS Magick++)

# Library source files
add_library(api      ${api_sources})
add_library(base64   ${base64_sources})
add_library(db       ${db_sources})
add_library(jsonrpc  ${jsonrpc_sources})
set_source_files_properties(
    src/mongoose.c
    PROPERTIES
    COMPILE_DEFINITIONS NS_ENABLE_SSL
    )
add_library(
    mongoose
    ${mongoose_sources}
    )
add_library(uri      ${uri_sources})
add_library(util     ${util_sources})
target_link_libraries(
    db
    base64
    )
target_link_libraries(
    mongoose
    "ssl"
    )

include_directories(src)
include_directories(commandline/include)
include_directories(json/include)
include_directories(sqlite/include)
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# Executables
add_executable(
    photoalbum
    ${photoalbum_sources}
    )
target_link_libraries(
    photoalbum
    api
    base64
    db
    jsonrpc
    mongoose
    uri
    util
    json
    sqlite
    "exiv2"
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    )

add_executable(
    exports
    ${exports_sources}
    )
target_link_libraries(
    exports
    db
    util
    json
    sqlite
    "exiv2"
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    )

add_executable(
    gazetteer
    ${gazetteer_sources}
    )
target_link_libraries(
    gazetteer
    base64
    db
    json
    sqlite
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    )

add_executable(
    streetmap
    ${streetmap_sources}
    )
target_link_libraries(
    streetmap
    base64
    db
    util
    json
    sqlite
    "exiv2"
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    )

