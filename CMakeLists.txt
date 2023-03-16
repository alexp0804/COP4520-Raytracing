set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required (VERSION 3.8)

# Add compiler flag for C++20 standard
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /MP /fp:fast")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread --ffast-math")
endif()

# Set the name of the project to be the same as its root folder
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

project(${PROJECT_NAME})

# Add source to this project's executable
add_executable(${PROJECT_NAME} "main.cpp" "camera.h" "color.h" "hittable.h" "hittable_list.h" "material.h" "ray.h" "rt_utilities.h" "sphere.h" "vec3.h")