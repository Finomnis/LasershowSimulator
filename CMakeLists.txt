cmake_minimum_required(VERSION 3.5)

project(LasershowSimulator)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 99)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -pedantic-errors")

# Search for dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

# Set include paths of dependencies
include_directories(${SDL2_INCLUDE_DIR})

# Project includes
include_directories(include)
include_directories(ext/include)
link_directories(ext/lib)

# Lasershows
aux_source_directory(shows SHOWS)

# Create executable
set(SRC
  src/main.cxx
  src/Log.hxx
  src/Log.cxx
  src/LasershowRenderer.hxx
  src/LasershowRenderer.cxx
  include/Lasershow.h
  src/Lasershow.c
  ext/glew/glew.c
  ${SHOWS}
)
add_executable(LasershowSimulator ${SRC})

# Link executable to dependencies
target_link_libraries(LasershowSimulator ${SDL2_LIBRARY} opengl32)
