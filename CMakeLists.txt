cmake_minimum_required(VERSION 2.8)
set( PROJ_NAME "LaserScanner" )
project( ${PROJ_NAME} )
find_package( OpenCV REQUIRED )

set( SRCS src/main.cpp
          src/extract_laser.cpp )

include_directories( include )

add_executable( ${PROJ_NAME} ${SRCS} )
target_link_libraries( ${PROJ_NAME} ${OpenCV_LIBS} )
