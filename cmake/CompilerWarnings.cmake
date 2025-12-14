# ==============================================================================
# CompilerWarnings.cmake
# Provides a function to set strict compiler warning flags per compiler
# ==============================================================================

function(set_project_warnings target)
    set(MSVC_WARNINGS
        /W4             # Baseline reasonable warnings
        /w14242         # 'identifier': conversion, possible loss of data
        /w14254         # 'operator': conversion, possible loss of data
        /w14263         # Member function doesn't override any base class virtual
        /w14265         # Class has virtual functions but destructor is not virtual
        /w14287         # Unsigned/negative constant mismatch
        /we4289         # Loop control variable used outside the for-loop scope
        /w14296         # Expression is always false
        /w14311         # Pointer truncation from one type to another
        /w14545         # Expression before comma evaluates to function
        /w14546         # Function call before comma missing argument list
        /w14547         # Operator before comma has no effect
        /w14549         # Operator before comma has no effect
        /w14555         # Expression has no effect
        /w14619         # Pragma warning: no such warning
        /w14640         # Static local object construction not thread-safe
        /w14826         # Conversion signed/unsigned mismatch
        /w14905         # Wide string literal cast to LPSTR
        /w14906         # String literal cast to LPWSTR
        /w14928         # Illegal copy-initialization
        /permissive-    # Strict standards conformance
    )

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
    )

    if(MSVC)
        set(PROJECT_WARNINGS_CXX ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
    else()
        message(AUTHOR_WARNING "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
    endif()

    target_compile_options(${target} PRIVATE ${PROJECT_WARNINGS_CXX})

    # Treat warnings as errors in Release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        if(MSVC)
            target_compile_options(${target} PRIVATE /WX)
        else()
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()
