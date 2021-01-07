cmake_minimum_required(VERSION 2.8)
project(draw_ellipse)

set(CMAKE_BUILD_TYPE Debug)
set( CMAKE_CXX_FLAGS "-std=c++11 -O0" )

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_DIRS}
        ${PROJECT_SOURCE_DIR}/include)

add_executable( run_draw_ellipse main.cpp src/DrawPreciseEllipse.cpp)
# 与Ceres和OpenCV链接
target_link_libraries( run_draw_ellipse ${OpenCV_LIBS} )
