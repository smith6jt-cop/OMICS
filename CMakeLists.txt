cmake_minimum_required(VERSION 3.20)

project(CellularThreshold 
    VERSION 0.1.0 
    LANGUAGES CXX
    DESCRIPTION "Cellular biology educational game engine"
)

# ==============================================================================
# C++ Standard Configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# Output Directories
# ==============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# Build Type Default
# ==============================================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# ==============================================================================
# Custom CMake Modules
# ==============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(ShaderCompilation)

# ==============================================================================
# Find Vulkan
# ==============================================================================
find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)
    message(STATUS "Vulkan found:")
    message(STATUS "  Include: ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "  Library: ${Vulkan_LIBRARIES}")
    message(STATUS "  Version: ${Vulkan_VERSION}")
else()
    message(FATAL_ERROR "Vulkan SDK not found. Install from https://vulkan.lunarg.com/")
endif()

# ==============================================================================
# Third-Party Dependencies
# ==============================================================================

# GLFW Configuration
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glfw/CMakeLists.txt")
    add_subdirectory(third_party/glfw)
    message(STATUS "GLFW: Using submodule")
else()
    message(FATAL_ERROR "GLFW not found. Run: git submodule update --init --recursive")
endif()

# GLM Configuration
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glm/CMakeLists.txt")
    add_subdirectory(third_party/glm)
    message(STATUS "GLM: Using submodule")
else()
    message(FATAL_ERROR "GLM not found. Run: git submodule update --init --recursive")
endif()

# ==============================================================================
# Engine Core Library
# ==============================================================================
add_library(engine_core STATIC
    # Core
    src/core/window.cpp
    src/core/engine.cpp
    # src/core/input.cpp           # Phase 2
    
    # Rendering
    src/rendering/vulkan_context.cpp
    # src/rendering/swapchain.cpp   # Phase 1.4
    # src/rendering/pipeline.cpp    # Phase 1.4
    
    # ECS (Phase 2)
    # src/ecs/entity_manager.cpp
    
    # Asset Pipeline (Phase 5)
    # src/asset_pipeline/asset_importer.cpp
)

target_include_directories(engine_core 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(engine_core
    PUBLIC
        Vulkan::Vulkan
        glfw
        glm::glm
)

# Apply compiler warnings to engine
set_project_warnings(engine_core)

# ==============================================================================
# Main Executable
# ==============================================================================
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        engine_core
)

set_project_warnings(${PROJECT_NAME})

# ==============================================================================
# Shader Compilation
# ==============================================================================
compile_shaders(
    TARGET ${PROJECT_NAME}
    SOURCES 
        ${CMAKE_SOURCE_DIR}/shaders/basic.vert
        ${CMAKE_SOURCE_DIR}/shaders/basic.frag
    OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)

# ==============================================================================
# Copy Assets (if any exist)
# ==============================================================================
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMENT "Copying assets to build directory"
    )
endif()

# ==============================================================================
# Installation (optional)
# ==============================================================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    DESTINATION bin
)

# ==============================================================================
# Build Summary
# ==============================================================================
message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "Project:    ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build:      ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler:   ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Platform:   ${CMAKE_SYSTEM_NAME}")
message(STATUS "Output:     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "")
