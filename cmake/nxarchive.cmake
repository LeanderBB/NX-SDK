#------------------------------------------------------------------------------
# Archiving utilties
#------------------------------------------------------------------------------

if(${NX_BUILD_SDK})
set(NX_ARCHIVE_CMD ${NX_3RDPARTY_DIR}/bin/yaafcl)
else()
find_program(NX_ARCHIVE_CMD yaafcl PATHS ${NX_SDK_DIR}/bin NO_DEFAULT_PATH)
if("${NX_ARCHIVE_CMD}" STREQUAL "NX_ARCHIVE_CMD-NOTFOUND")
    message(SEND_ERROR "Could not find yaafcl")
else()
    message(STATUS "Found yaafcl: ${NX_ARCHIVE_CMD}")
endif()
endif()

function(NX_CREATE_ARCHIVE)
    set(options)
    set(oneValueArgs TARGET ARCHIVE)
    set(multiValueArgs DIRS)
    cmake_parse_arguments(NX_CREATE_ARCHIVE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    add_dependencies(${NX_CREATE_ARCHIVE_TARGET} YAAF)
    add_custom_command(
        TARGET ${NX_CREATE_ARCHIVE_TARGET} POST_BUILD
        COMMAND ${NX_ARCHIVE_CMD} -c -r ${NX_CREATE_ARCHIVE_ARCHIVE} ${NX_CREATE_ARCHIVE_DIRS}
        WORKING_DIRECTORY ${cwd}
        COMMENT "Archiving ${NX_CREATE_ARCHIVE_DIRS} for target ${NX_CREATE_ARCHIVE_TARGET}"
    )
endfunction()
