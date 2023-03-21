file(GLOB src src/*.cpp src/*.c src/*.cc)
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${CURRENT_FOLDER} OBJECT ${src})