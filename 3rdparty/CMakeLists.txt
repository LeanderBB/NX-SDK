#------------------------------------------------------------------------------
# 3rd Party Dependencies
#------------------------------------------------------------------------------

include(ExternalProject)

set(NX_3RDPARTY_DIR ${CMAKE_BINARY_DIR}/sysroot_3rdparty)
set(NX_3RDPARTY_DIR ${NX_3RDPARTY_DIR} PARENT_SCOPE)

if(DEFINED NX_CROSS_COMPILE)
    set(NX_3RDPARTY_TOOLCHAIN -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
    if(ANDROID)
        set(NX_3RDPARTY_TOOLCHAIN
            -DANDROID_TOOLCHAIN_DIR=${ANDROID_TOOLCHAIN_DIR}
            -DANDROID_SDK_DIR=${ANDROID_SDK_DIR}
            ${NX_3RDPARTY_TOOLCHAIN}
        )
        set(NX_3RDPARTY_C_FLAGS "${ANDROID_C_FLAGS}")
        set(NX_3RDPARTY_CXX_FLAGS "${ANDROID_CXX_FLAGS}")
    endif()
endif()

# ---- GLM ---------------------------------------------------------------------
ExternalProject_Add( glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "0.9.6.1"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glm"
    CMAKE_ARGS  ${NX_3RDPARTY_TOOLCHAIN} -DCMAKE_INSTALL_PREFIX=${NX_3RDPARTY_DIR} -DCMAKE_C_FLAGS=${NX_3RDPARTY_C_FLAGS} -DCMAKE_CXX_FLAGS=${NX_3RDPARTY_CXX_FLAGS}
    INSTALL_DIR ${NX_3RDPARTY_DIR}
)

# ---- YAAF --------------------------------------------------------------------
if(DEFINED NX_CROSS_COMPILE)
set(YAAF_CROSS_COMPILE_OPTIONS -DYAAF_EXCLUDE_YAAFCL=1)
endif()

ExternalProject_Add( YAAF
    GIT_REPOSITORY "https://github.com/LeanderBB/YAAF.git"
    GIT_TAG "v1.1.2"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/YAAF"
    CMAKE_ARGS ${NX_3RDPARTY_TOOLCHAIN} ${YAAF_CROSS_COMPILE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${NX_3RDPARTY_DIR} -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=${NX_3RDPARTY_C_FLAGS} -DCMAKE_CXX_FLAGS=${NX_3RDPARTY_CXX_FLAGS}
    INSTALL_DIR ${NX_3RDPARTY_DIR}
)

set(NX_YAAF_LIBRARIES -lYAAF -lYAAF-lz4-xxhash)
set(NX_YAAF_LIBRARIES ${NX_YAAF_LIBRARIES} PARENT_SCOPE)


# ---- FlatBuffers  ------------------------------------------------------------
if(DEFINED NX_CROSS_COMPILE)
set(FLATBUFFER_CROSS_COMPILE_OPTIONS -DFLATBUFFERS_BUILD_CL=OFF)
endif()

ExternalProject_Add( FlatBuffers
    GIT_REPOSITORY "https://github.com/LeanderBB/flatbuffers.git"
    GIT_TAG "DisableBuildingOfCompiler"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/FlatBuffers"
    CMAKE_ARGS ${NX_3RDPARTY_TOOLCHAIN} ${FLATBUFFER_CROSS_COMPILE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${NX_3RDPARTY_DIR} -DFLATBUFFERS_BUILD_TESTS=Off -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=${NX_3RDPARTY_C_FLAGS} -DCMAKE_CXX_FLAGS=${NX_3RDPARTY_CXX_FLAGS}
    INSTALL_DIR ${NX_3RDPARTY_DIR}
)

if(NOT DEFINED NX_CROSS_COMPILE)
    set(NX_FLATBUFFERS_COMPILER ${CMAKE_BINARY_DIR}/sysroot_3rdparty/bin/flatc)
    set(NX_FLATBUFFERS_COMPILER ${NX_FLATBUFFERS_COMPILER} PARENT_SCOPE)
else()
    if(NOT DEFINED NX_FLATBUFFER_HOST_CL)
        message(SEND_ERROR "Cross compiling, please define a host build of flatc with NX_FLATBUFFER_HOST_CL")
    else()
        set(NX_FLATBUFFERS_COMPILER ${NX_FLATBUFFER_HOST_CL} PARENT_SCOPE)
    endif()
endif()

macro(NX_COMPILE_FLATBUFFER_IDL TARGET OUTPUT_DIR INPUT_DIR FILES)
    include_directories(${OUTPUT_DIR})
    set(${TARGET}_generated_files)
    foreach(FILE ${${FILES}})
        add_custom_command(
            OUTPUT ${OUTPUT_DIR}/${FILE}_generated.h
            COMMAND ${NX_FLATBUFFERS_COMPILER} -c -o ${OUTPUT_DIR} ${INPUT_DIR}/${FILE}.idl
            DEPENDS ${INPUT_DIR}/${FILE}.idl FlatBuffers
            COMMENT "Compiling IDL ${INPUT_DIR}/${FILE}.idl"
        )
        set(${TARGET}_generated_files ${${TARGET}_generated_files} ${OUTPUT_DIR}/${FILE}_generated.h)
    endforeach()
    add_custom_target(${PROJ_NAME}_idl DEPENDS ${${TARGET}_generated_files})
    set_source_files_properties(${OUTPUT_DIR}/${FILE}_generated.h PROPERTIES GENERATED 1)
    add_dependencies(${TARGET} ${PROJ_NAME}_idl)
endmacro()


# ---- GLFW --------------------------------------------------------------------
if(NOT DEFINED NX_CROSS_COMPILE AND DEFINED NX_CONFIG_USE_GLFW)

ExternalProject_Add( GLFW
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.1"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw"
    CMAKE_ARGS ${NX_3RDPARTY_TOOLCHAIN}  -DCMAKE_INSTALL_PREFIX=${NX_3RDPARTY_DIR} -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=${NX_3RDPARTY_C_FLAGS} -DCMAKE_CXX_FLAGS=${NX_3RDPARTY_CXX_FLAGS} -DGLFW_BUILD_EXAMPLES=Off -DGLFW_BUILD_TESTS=Off -DGLFW_BUILD_DOCS=Off
    INSTALL_DIR ${NX_3RDPARTY_DIR}
)

set(NX_GLFW_LIBRARIES glfw3)
#FIXME: Find an automatic way to collect this data
if(UNIX AND NOT APPLE)
set(NX_GLFW_LIBRARIES ${NX_GLFW_LIBRARIES} X11 Xxf86vm Xrandr Xi Xcursor Xinerama)
endif()

set(NX_GLFW_LIBRARIES ${NX_GLFW_LIBRARIES} PARENT_SCOPE)
endif()

# ---- SDL2 --------------------------------------------------------------------
if(NOT DEFINED NX_CROSS_COMPILE AND NOT DEFINED NX_CONFIG_USE_GLFW)

ExternalProject_Add(NXSDL2
    HG_REPOSITORY "http://hg.libsdl.org/SDL"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2"
    CMAKE_ARGS ${NX_3RDPARTY_TOOLCHAIN} -DCMAKE_INSTALL_PREFIX=${NX_3RDPARTY_DIR} -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=${NX_3RDPARTY_C_FLAGS} -DCMAKE_CXX_FLAGS=${NX_3RDPARTY_CXX_FLAGS} -DVIDEO_WAYLAND=Off -DALSA_SHARED=Off -DPULSEAUDIO_SHARED=Off -DX11_SHARED=Off -DFUSIONSOUND_SHARED=Off -DDIRECTFB_SHARED=Off -DESD_SHARED=Off -DARTS_SHARED=Off -DNAS_SHARED=Off -DSDL_DLOPEN=On
    INSTALL_DIR ${NX_3RDPARTY_DIR}
)

set(NX_SDL2_LIBRARIES SDL2)
#FIXME: Find an automatic way to collect this data
if(UNIX AND NOT APPLE)
set(NX_SDL2_LIBRARIES ${NX_SDL2_LIBRARIES} X11 Xxf86vm Xrandr Xi Xcursor Xinerama asound Xss)
endif()

set(NX_SDL2_LIBRARIES ${NX_SDL2_LIBRARIES} PARENT_SCOPE)
endif()
# ---- Install settings --------------------------------------------------------
install(DIRECTORY ${NX_3RDPARTY_DIR}/include/ DESTINATION ${NX_HOST_PREFIX}/include)
install(DIRECTORY ${NX_3RDPARTY_DIR}/lib/ DESTINATION ${NX_HOST_PREFIX}/lib)
if(NOT DEFINED NX_CROSS_COMPILE)
install(DIRECTORY ${NX_3RDPARTY_DIR}/bin/ DESTINATION bin
        PATTERN "*"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)
endif()
