cmake_minimum_required(VERSION 2.6)

set(SQLITE_JSON_LIBRARY_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(json)
add_subdirectory(sqlite)

add_subdirectory(web)

project(PhotoAlbum)

set(CMAKE_CXX_FLAGS "-std=c++0x")
find_package(Threads)
find_package(Boost COMPONENTS filesystem regex)
find_package(ImageMagick COMPONENTS Magick++)

file(GLOB sources src/*.cpp)
file(GLOB sources src/*.c)

file(GLOB api_sources     src/api/*.cpp)
file(GLOB base64_sources  src/base64/*.cpp)
file(GLOB db_sources      src/db/*.cpp)
file(GLOB jsonrpc_sources src/jsonrpc/*.cpp)
file(GLOB uri_sources     src/uri/*.cpp)
file(GLOB util_sources    src/util/*.cpp)

add_library(api     ${api_sources})
add_library(db      ${db_sources})
add_library(jsonrpc ${jsonrpc_sources})
add_library(uri     ${uri_sources})
add_library(util    ${util_sources})

include_directories(src)
include_directories(commandline/include)
include_directories(json/include)
include_directories(sqlite/include)
include_directories(${ImageMagick_INCLUDE_DIRS})

add_executable(
    photoalbum
    ${sources}
    )

target_link_libraries(
    photoalbum
    api
    db
    jsonrpc
    uri
    util

    json
    sqlite

    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    )

