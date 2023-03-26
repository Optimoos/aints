cmake_minimum_required(VERSION 3.22)
project(aiants CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_VERSION VERSION_GREATER "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        #URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


file(GLOB AINTS_FILES "include/*.h" "src/*.cpp")
add_subdirectory(lib/entt lib/entt/src SYSTEM)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
add_subdirectory(lib/raylib/projects/CMake SYSTEM)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build raygui examples
add_subdirectory(lib/raygui/projects/CMake SYSTEM)
set(FASTNOISE2_NOISETOOL OFF CACHE BOOL "Build Noise Tool" FORCE)
add_subdirectory(lib/FastNoise2 SYSTEM)
include_directories(SYSTEM include src lib/thread-pool lib/astar-algorithm-cpp/cpp)
set(SOURCES main.cpp)
add_executable(aiants main.cpp ${AINTS_FILES})

#add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib raygui EnTT::EnTT FastNoise)
#target_link_libraries(aints_test GTest)
target_include_directories(${PROJECT_NAME} PRIVATE include/)

enable_testing()

add_executable(
        aints_test
        src/tests/aints_test.cpp
        ${AINTS_FILES}
)
target_link_libraries(
        aints_test
        raylib
        raygui
        EnTT::EnTT
        FastNoise
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(aints_test)
