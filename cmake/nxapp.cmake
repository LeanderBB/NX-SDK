#------------------------------------------------------------------------------
# NXApp packaging script
#------------------------------------------------------------------------------

include(nxarchive)
include(nxresources)
include(nxcrosscompile)

if(ANDROID)
include(nxandroid)
endif()


function(NX_CREATE_APP)
    set(options SKIP_ARCHIVE)
    set(oneValueArgs NAME EXEC_NAME DATA_DIR SHADER_DIR ARCHIVE_NAME PACKAGE)
    set(multiValueArgs SOURCES HEADERS ARCHIVE_DIRS PROGRAMS)

    cmake_parse_arguments(NX_CREATE_APP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(ANDROID)
        NX_ANDROID_MKDIRS()
        add_library(${NX_CREATE_APP_EXEC_NAME} SHARED ${NX_CREATE_APP_SOURCES} ${NX_CREATE_APP_HEADERS})
        NX_ANDROID_GEN_FILES(${NX_CREATE_APP_EXEC_NAME} ${NX_CREATE_APP_NAME})
    else()
        add_executable(${NX_CREATE_APP_EXEC_NAME} ${NX_CREATE_APP_SOURCES} ${NX_CREATE_APP_HEADERS})
    endif()

    NX_PROCESS_GPUPROGRAMS(
        ${NX_CREATE_APP_EXEC_NAME}
        ${NX_CREATE_APP_DATA_DIR}
        ${NX_CREATE_APP_SHADER_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/programs
        ${NX_CREATE_APP_PROGRAMS}
    )

    if(NOT DEFINED ANDROID AND (NOT "" STREQUAL "${NX_CREATE_APP_ARCHIVE_NAME}"))
        set(__ARCHIVE_NAME ${NX_CREATE_APP_ARCHIVE_NAME})
    else()
        string(TOLOWER ${NX_CREATE_APP_NAME} __ARCHIVE_NAME)
    endif()


    if(NOT NX_CREATE_APP_SKIP_ARCHIVE)
        set(__ARCHIVE_DIRS ${NX_CREATE_APP_ARCHIVE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/programs)
        NX_CREATE_ARCHIVE(
            TARGET ${NX_CREATE_APP_EXEC_NAME}
            ARCHIVE ${CMAKE_CURRENT_BINARY_DIR}/${__ARCHIVE_NAME}.yaaf
            DIRS ${__ARCHIVE_DIRS}
        )
    endif()
endfunction()
