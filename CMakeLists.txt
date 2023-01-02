cmake_minimum_required(VERSION 3.22)
project(aiants CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/entt lib/entt/src)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
add_subdirectory(lib/raylib lib/raylib/bin)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build raygui examples
add_subdirectory(lib/raygui/projects/CMake)
set(SOURCES main.cpp)
add_executable(aiants main.cpp aints.cpp aints.h)

#add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib raygui EnTT::EnTT)
target_include_directories(${PROJECT_NAME} PRIVATE include/)