# ==============================================================================
# ShaderCompilation.cmake
# Provides a function to compile GLSL shaders to SPIR-V
# ==============================================================================

# Find glslc shader compiler (part of Vulkan SDK)
find_program(GLSLC_EXECUTABLE glslc
    HINTS
        $ENV{VULKAN_SDK}/bin
        $ENV{VULKAN_SDK}/Bin
)

if(NOT GLSLC_EXECUTABLE)
    message(WARNING "glslc not found. Shaders will not be compiled automatically.")
endif()

# ==============================================================================
# compile_shaders(TARGET target SOURCES shader1.vert shader2.frag ... OUTPUT_DIR dir)
#
# Compiles GLSL shaders to SPIR-V and adds them as dependencies to the target.
# ==============================================================================
function(compile_shaders)
    set(options "")
    set(oneValueArgs TARGET OUTPUT_DIR)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(SHADER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT SHADER_TARGET)
        message(FATAL_ERROR "compile_shaders: TARGET not specified")
    endif()

    if(NOT SHADER_OUTPUT_DIR)
        set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")
    endif()

    # Ensure output directory exists
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    if(NOT GLSLC_EXECUTABLE)
        message(WARNING "glslc not found - skipping shader compilation")
        return()
    endif()

    set(SPIRV_OUTPUTS "")

    foreach(SHADER_SOURCE ${SHADER_SOURCES})
        get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME)
        set(SPIRV_OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv")

        add_custom_command(
            OUTPUT ${SPIRV_OUTPUT}
            COMMAND ${GLSLC_EXECUTABLE}
                    -o ${SPIRV_OUTPUT}
                    ${SHADER_SOURCE}
            DEPENDS ${SHADER_SOURCE}
            COMMENT "Compiling shader: ${SHADER_NAME} -> ${SHADER_NAME}.spv"
            VERBATIM
        )

        list(APPEND SPIRV_OUTPUTS ${SPIRV_OUTPUT})
    endforeach()

    # Create a custom target for shader compilation
    set(SHADER_TARGET_NAME "${SHADER_TARGET}_shaders")
    add_custom_target(${SHADER_TARGET_NAME} DEPENDS ${SPIRV_OUTPUTS})

    # Make the main target depend on shaders
    add_dependencies(${SHADER_TARGET} ${SHADER_TARGET_NAME})

    message(STATUS "Shader compilation configured:")
    message(STATUS "  Target: ${SHADER_TARGET}")
    message(STATUS "  Output: ${SHADER_OUTPUT_DIR}")
    message(STATUS "  Shaders: ${SHADER_SOURCES}")
endfunction()
