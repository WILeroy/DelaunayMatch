cmake_minimum_required( VERSION 2.8 )
project( delaunayDetect )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

find_package( OpenCV 3.2 REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
)

add_executable( delaunayDetect delaunay_match.cpp _c_triangle_net.cpp )

target_link_libraries( delaunayDetect ${OpenCV_LIBS} )
