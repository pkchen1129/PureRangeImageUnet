#cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_minimum_required(VERSION 3.9.4)
project(ROB530_PCL)
find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
include_directories(/usr/include/eigen3)

add_executable(visualize_pc visualize_pc.cpp)
target_link_libraries(visualize_pc ${PCL_LIBRARIES})

add_executable(visualize_rimg_pc visualize_rimg_pc.cpp)
target_link_libraries(visualize_rimg_pc ${PCL_LIBRARIES})

add_executable(write_rimg_bin write_rimg_bin.cpp)
target_link_libraries(write_rimg_bin ${PCL_LIBRARIES})

add_executable(write_rimg_labels write_rimg_labels.cpp)
target_link_libraries(write_rimg_labels ${PCL_LIBRARIES})

add_executable(read_rimg_bin read_rimg_bin.cpp)
target_link_libraries(read_rimg_bin ${PCL_LIBRARIES})

add_executable(filepath filepath.cpp)
target_link_libraries(filepath ${PCL_LIBRARIES})

add_executable(write_rimg_vo_bin write_rimg_vo_bin.cpp)
target_link_libraries(write_rimg_vo_bin ${PCL_LIBRARIES})

add_executable(write_rimg_vo_labels write_rimg_vo_labels.cpp)
target_link_libraries(write_rimg_vo_labels ${PCL_LIBRARIES})

SET(COMPILE_FLAGS "-std=c++17")
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
add_definitions(${COMPILE_FLAGS})
