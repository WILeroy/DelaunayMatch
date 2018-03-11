cmake_minimum_required( VERSION 2.8 )
project( delaunayDetect )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

find_package( OpenCV 3.2 REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
)

add_executable( delaunayDetect delaunaydetect.cpp _c_trianglenet.cpp )

target_link_libraries( delaunayDetect ${OpenCV_LIBS} )
