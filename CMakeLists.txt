cmake_minimum_required(VERSION 3.23)
project(Test_Project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Test_Project main.cpp Game.cpp Game.h Block.cpp Block.h)

# Statically load the library. (Copy the files over directly instead of dynamically linking them)
# This means we don't load them at run time every time.
set(SFML_STATIC_LIBRARIES TRUE)
# Requires SFML to be found at the location specified.
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)
target_link_libraries(Test_Project sfml-system sfml-window sfml-graphics sfml-audio)
