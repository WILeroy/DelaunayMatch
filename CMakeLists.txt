cmake_minimum_required( VERSION 2.8 )
project( DelaunayMatch )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

find_package( OpenCV 3.2 REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
)

add_executable( DelaunayMatch delaunay_match.cpp _c_triangle_net.cpp _c_triangle.cpp _sc_calculator.cpp)

target_link_libraries( DelaunayMatch ${OpenCV_LIBS} )
