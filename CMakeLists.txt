cmake_minimum_required(VERSION 3.16)
project(Isometric VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================
# Output directories
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================
# Source files
# ============================================================
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

# Include main.cpp at root if present
if(EXISTS ${CMAKE_SOURCE_DIR}/main.cpp)
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/main.cpp)
endif()

add_executable(${PROJECT_NAME} ${SOURCES}
        include/window.h)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# OpenGL
# ============================================================
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# ============================================================
# GLFW  (window + context creation)
# Tries system install, then vendor/, then downloads via FetchContent
# ============================================================
find_package(glfw3 3.3 QUIET)

if(glfw3_FOUND)
    message(STATUS "Found system GLFW")
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/vendor/glfw/CMakeLists.txt)
    message(STATUS "Using vendored GLFW")
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
else()
    message(STATUS "GLFW not found locally — fetching via FetchContent")
    include(FetchContent)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(glfw)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
endif()

# ============================================================
# GLAD  (OpenGL function loader)
# Expects vendor/glad/ with glad.c + include/glad/glad.h
# Generate at: https://glad.dav1d.de  (gl, core, your GL version)
# ============================================================
if(EXISTS ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c)
    add_library(glad STATIC
        vendor/glad/src/glad.c
    )
    target_include_directories(glad PUBLIC vendor/glad/include)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad)
else()
    message(FATAL_ERROR
        "GLAD not found. Generate it at https://glad.dav1d.de and place it in vendor/glad/\n"
        "  Expected: vendor/glad/src/glad.c and vendor/glad/include/glad/glad.h")
endif()

# ============================================================
# GLM  (math library — header only)
# Tries system install, then vendor/, then downloads via FetchContent
# ============================================================
find_package(glm QUIET)

if(glm_FOUND)
    message(STATUS "Found system GLM")
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/vendor/glm/CMakeLists.txt)
    message(STATUS "Using vendored GLM")
    add_subdirectory(vendor/glm)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/vendor/glm/glm/glm.hpp)
    message(STATUS "Using vendored GLM (header-only)")
    target_include_directories(${PROJECT_NAME} PRIVATE vendor/glm)
else()
    message(STATUS "GLM not found locally — fetching via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(glm)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

# ============================================================
# Platform-specific libraries
# ============================================================
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        gdi32
        user32
        shell32
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

# ============================================================
# Compiler warnings
# ============================================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
