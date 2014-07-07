cmake_minimum_required(VERSION 2.8)

configure_file(package.json package.json COPYONLY)
add_custom_target(
    node_modules
    COMMAND npm install
    )
# Copy the client source to the web directory to be picked up by Webmake.
add_custom_target(
    copy_web
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/client ${CMAKE_CURRENT_BINARY_DIR}/client
    )
add_custom_target(
    copy_favicon
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/static/favicon.png ${CMAKE_CURRENT_BINARY_DIR}/static/favicon.png
    )
add_custom_target(
    bundle
    DEPENDS node_modules copy_favicon copy_web
    COMMAND
        node_modules/webmake/bin/webmake
        ${CMAKE_CURRENT_BINARY_DIR}/client/index.js
        ${CMAKE_CURRENT_BINARY_DIR}/static/bundle.js
    )

configure_file(static/index.html static/index.html COPYONLY)
configure_file(static/pure-min.css static/pure-min.css COPYONLY)
configure_file(
    static/open-iconic/font/css/open-iconic.css
    static/open-iconic/font/css/open-iconic.css
    COPYONLY
    )
configure_file(
    static/open-iconic/font/fonts/open-iconic.ttf
    static/open-iconic/font/fonts/open-iconic.ttf
    COPYONLY
    )

