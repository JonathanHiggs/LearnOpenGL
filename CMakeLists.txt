cmake_minimum_required(VERSION 3.16)

IF(DEFINED ENV[VCPKG_DIR] AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_DIR}/scripts/buildsystem/vcpkg.cmake"
        CACHE STRING "")
ENDIF()

project(LearnOpenGL VERSION 0.1)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "/EHsc /WX /W4 /wd4100 /wd4189")
set(CMAKE_CXX_FLAGS_DEBUG "/Od /Oi /Zi /JMC")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CMake")

add_definitions(-DGLM_FORCE_SILENT_WARNINGS=1)
add_definitions(-DGLFW_INCLUDE_NONE)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)

include_directories(
    Vendor/Glad/include
    Vendor/stb/include
)

add_subdirectory(Breakout)
add_subdirectory(src)
add_subdirectory(Vendor)


set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT LearnOpenGL)