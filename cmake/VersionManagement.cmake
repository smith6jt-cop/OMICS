# ==============================================================================
# VersionManagement.cmake
# Handles version reading from VERSION file and generates version header
# ==============================================================================

# Read version from VERSION file or use CT_VERSION if provided (CI override)
function(configure_project_version)
    # Check for CI-provided version first
    if(DEFINED CT_VERSION)
        set(VERSION_STRING "${CT_VERSION}")
        message(STATUS "Using CI-provided version: ${VERSION_STRING}")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/VERSION")
        file(READ "${CMAKE_SOURCE_DIR}/VERSION" VERSION_STRING)
        string(STRIP "${VERSION_STRING}" VERSION_STRING)
        message(STATUS "Read version from VERSION file: ${VERSION_STRING}")
    else()
        set(VERSION_STRING "0.0.0")
        message(WARNING "VERSION file not found, using default: ${VERSION_STRING}")
    endif()

    # Parse version components
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)(-([a-zA-Z0-9.]+))?"
           VERSION_MATCH "${VERSION_STRING}")

    if(NOT VERSION_MATCH)
        message(FATAL_ERROR "Invalid version format: ${VERSION_STRING}")
    endif()

    set(CT_VERSION_MAJOR "${CMAKE_MATCH_1}" PARENT_SCOPE)
    set(CT_VERSION_MINOR "${CMAKE_MATCH_2}" PARENT_SCOPE)
    set(CT_VERSION_PATCH "${CMAKE_MATCH_3}" PARENT_SCOPE)
    set(CT_VERSION_PRERELEASE "${CMAKE_MATCH_5}" PARENT_SCOPE)
    set(CT_VERSION_FULL "${VERSION_STRING}" PARENT_SCOPE)

    # Set project version
    set(PROJECT_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}" PARENT_SCOPE)
    set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}" PARENT_SCOPE)
    set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_2}" PARENT_SCOPE)
    set(PROJECT_VERSION_PATCH "${CMAKE_MATCH_3}" PARENT_SCOPE)

    message(STATUS "Version: ${VERSION_STRING}")
    message(STATUS "  Major: ${CMAKE_MATCH_1}")
    message(STATUS "  Minor: ${CMAKE_MATCH_2}")
    message(STATUS "  Patch: ${CMAKE_MATCH_3}")
    if(CMAKE_MATCH_5)
        message(STATUS "  Pre-release: ${CMAKE_MATCH_5}")
    endif()
endfunction()

# Generate version header file
function(generate_version_header OUTPUT_DIR)
    set(VERSION_HEADER_CONTENT
"// Auto-generated version header - DO NOT EDIT
#pragma once

namespace ct {
namespace version {

constexpr int MAJOR = ${CT_VERSION_MAJOR};
constexpr int MINOR = ${CT_VERSION_MINOR};
constexpr int PATCH = ${CT_VERSION_PATCH};

constexpr const char* STRING = \"${CT_VERSION_FULL}\";
constexpr const char* PRERELEASE = \"${CT_VERSION_PRERELEASE}\";

// Compile-time version check
constexpr int VERSION_NUMBER = (MAJOR * 10000) + (MINOR * 100) + PATCH;

} // namespace version
} // namespace ct
")

    file(WRITE "${OUTPUT_DIR}/version.h" "${VERSION_HEADER_CONTENT}")
    message(STATUS "Generated version header: ${OUTPUT_DIR}/version.h")
endfunction()

# Get git information for build metadata
function(get_git_info)
    find_package(Git QUIET)

    if(GIT_FOUND)
        # Get commit hash
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        # Get branch name
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        # Check for uncommitted changes
        execute_process(
            COMMAND ${GIT_EXECUTABLE} status --porcelain
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_STATUS
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )

        if(GIT_STATUS)
            set(GIT_DIRTY "+dirty" PARENT_SCOPE)
        else()
            set(GIT_DIRTY "" PARENT_SCOPE)
        endif()

        set(GIT_COMMIT_HASH "${GIT_COMMIT_HASH}" PARENT_SCOPE)
        set(GIT_BRANCH "${GIT_BRANCH}" PARENT_SCOPE)

        message(STATUS "Git info: ${GIT_BRANCH}@${GIT_COMMIT_HASH}${GIT_DIRTY}")
    else()
        set(GIT_COMMIT_HASH "unknown" PARENT_SCOPE)
        set(GIT_BRANCH "unknown" PARENT_SCOPE)
        set(GIT_DIRTY "" PARENT_SCOPE)
    endif()
endfunction()
