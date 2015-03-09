#------------------------------------------------------------------------------
# Resource creation
#------------------------------------------------------------------------------

if(NOT NX_BUILD_SDK)
    find_program(nxprogrambuilder_bin nxprogrambuilder PATHS ${NX_SDK_DIR}/bin NO_DEFAULT_PATH)
    if("${nxprogrambuilder_bin}" STREQUAL "nxprogrambuilder_bin-NOTFOUND")
        message(SEND_ERROR "Could not find nxprogrambuilder")
    else()
        message(STATUS "Found nxprogrambuilder: ${nxprogrambuilder_bin}")
    endif()
endif()

# NX_PROCESS_GPUPROGRAMS - Convert GPU Programs into nxprog files
# TARGET - target to register dependency
# INFOLDER - location of the .prog files
# INFOLDER_SHADERS - location of the shaders listed in the .prog
# OUTFOLDER - Output location
# NAMES - List of program names
function(NX_PROCESS_GPUPROGRAMS)
    set(options)
    set(oneValueArgs TARGET INFOLDER INFOLDER_SHADERS OUTFOLDER)
    set(multiValueArgs NAMES)

    cmake_parse_arguments(NX_PROCESS_GPUPROGRAMS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    file(MAKE_DIRECTORY ${NX_PROCESS_GPUPROGRAMS_OUTFOLDER})
    set(OUTFILES)
    foreach(PROG ${NX_PROCESS_GPUPROGRAMS_NAMES})
        set(INPUT_NAME  ${NX_PROCESS_GPUPROGRAMS_INFOLDER}/${PROG}.prog)
        set(OUTPUT_NAME ${NX_PROCESS_GPUPROGRAMS_OUTFOLDER}/${PROG}.nxprog)
        add_custom_command(
            TARGET ${NX_PROCESS_GPUPROGRAMS_TARGET} PRE_BUILD
            COMMAND ${nxprogrambuilder_bin} -d ${INPUT_NAME} -s ${NX_PROCESS_GPUPROGRAMS_INFOLDER_SHADERS} -o ${OUTPUT_NAME}
            DEPENDS ${INPUT_NAME}
            COMMENT "Creating ${OUTPUT_NAME}...")
        set(OUTFILES ${OUTFILES} ${OUTPUT_NAME})
    endforeach()
endfunction()
