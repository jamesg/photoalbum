cmake_minimum_required(VERSION 2.6)

set(SQLITE_JSON_LIBRARY_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(json)
add_subdirectory(sqlite)

add_subdirectory(web)

project(PhotoAlbum)

set(CMAKE_CXX_FLAGS "-std=c++0x")

file(GLOB exports_sources    src/exports/*.cpp)
file(GLOB photoalbum_sources src/photoalbum/*.cpp)
file(GLOB sources            src/*.c src/*.cpp)

file(GLOB api_sources     src/api/*.cpp src/api/*/*.cpp)
file(GLOB base64_sources  src/base64/*.cpp)
file(GLOB db_sources      src/db/*.cpp src/db/*/*.cpp)
file(GLOB jsonrpc_sources src/jsonrpc/*.cpp)
file(GLOB uri_sources     src/uri/*.cpp)
file(GLOB util_sources    src/util/*.cpp)

find_package(Threads)
find_package(Boost COMPONENTS date_time filesystem random regex system)
find_package(ImageMagick COMPONENTS Magick++)

add_library(api     ${api_sources})
add_library(base64  ${base64_sources})
add_library(db      ${db_sources})
add_library(jsonrpc ${jsonrpc_sources})
add_library(uri     ${uri_sources})
add_library(util    ${util_sources})

include_directories(src)
include_directories(commandline/include)
include_directories(json/include)
include_directories(sqlite/include)
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(
    photoalbum
    ${photoalbum_sources}
    ${sources}
    )

target_link_libraries(
    photoalbum
    api
    base64
    db
    jsonrpc
    uri
    util
    json
    sqlite
    "exiv2"
    "ssl"
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${ImageMagick_LIBRARIES}
    )

add_executable(
    exports
    ${exports_sources}
    ${sources}
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

