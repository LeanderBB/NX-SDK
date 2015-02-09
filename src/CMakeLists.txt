#------------------------------------------------------------------------------
# NX Project
#------------------------------------------------------------------------------

set(PROJ_NAME nx)

#------------------------------------------------------------------------------
# dependencies
#------------------------------------------------------------------------------

# SDL 2
if(NOT DEFINED ANDROID)
    set(NX_SYSTEM_SDL2 1 CACHE INTERNAL "Use System SDL2")
    find_package(SDL2 REQUIRED)
    include_directories(/usr/local/include)
    # OpenGL
    find_package(OpenGL REQUIRED)
else()
    set(NX_SYSTEM_ANDROID 1 CACHE INTERNAL "Use System Android")
    set(OPENGL_LIBRARIES EGL GLESv2)
endif()

# check string functions
check_include_files(strings.h NX_HAVE_STRINGS_H)
if(${NX_HAVE_STRINGS_H})
    set(CMAKE_REQUIRED_INCLUDES string.h strings.h)
else()
    set(CMAKE_REQUIRED_INCLUDES string.h)
endif()

check_function_exists(strcasecmp NX_HAVE_STRCASECMP)
check_function_exists(stricmp NX_HAVE_STRICMP)
unset(CMAKE_REQUIRED_INCLUDES)

# check dirent
check_include_files(dirent.h NX_HAVE_DIRENT_H)

#check inttypes
check_include_files(inttypes.h NX_HAVE_INTTYPES_H)

# header config file
set(NX_CONFIG_HEADER_DIR ${PROJECT_BINARY_DIR}/build_config)
set(NX_CONFIG_FILE "${NX_CONFIG_HEADER_DIR}/nx/nxconfig.h")
set(NX_CONFIG_FILE_CMAKE "${NX_CONFIG_HEADER_DIR}/cmake/nxconfig.cmake")

configure_file("cmake/nxconfig.h.cmake" "${NX_CONFIG_FILE}")



#------------------------------------------------------------------------------
# includes
#------------------------------------------------------------------------------

set(NXCORE_INCLUDE_DIRS
${CMAKE_CURRENT_SOURCE_DIR}
${SDL2_INCLUDE_DIR}
${NX_CONFIG_HEADER_DIR}
${OPENGL_INCLUDE_DIRS}
${CMAKE_CURRENT_BINARY_DIR}/genidl
)


set(NXCORE_LIBRARIES
${PROJ_NAME}
${SDL2_LIBRARY}
${OPENGL_LIBRARIES}
${NX_YAAF_LIBRARIES}
${ANDROID_LIBS}
)

if(UNIX)
set(NXCORE_LIBRARIES ${NXCORE_LIBRARIES} dl)
endif()

set(NXCORE_INCLUDE_DIRS ${NXCORE_INCLUDE_DIRS} PARENT_SCOPE)
set(NXCORE_LIBRARIES ${NXCORE_LIBRARIES} PARENT_SCOPE)


include_directories(
    ${NXCORE_INCLUDE_DIRS}
)


#------------------------------------------------------------------------------
# FLatbuffers IDL
#------------------------------------------------------------------------------
set(PROJ_IDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/idl)

set(PROJ_IDL
    nxgpuprogramsource
    nx3dmodel
)


#------------------------------------------------------------------------------
# sources
#------------------------------------------------------------------------------


set(PROJ_HDR
    nx/nxcore.h
    nx/config/nxsetup.h
    nx/config/nxcpp.h
    nx/allocator/nxmemory.h
    nx/allocator/nxalloctracker.h
    nx/log/nxlog.h
    nx/os/nxatomic.h
    nx/os/nxthread.h
    nx/os/sdl2/nxthreadsdl.h
    nx/os/sdl2/nxlocksdl.h
    nx/os/posix/nxthreadposix.h
    nx/os/posix/nxlockposix.h
    nx/os/nxlock.h
    nx/os/nxpath.h
    nx/os/nxsharedobj.h
    nx/os/nxargs.h
    nx/util/nxsignal.h
    nx/util/nxtime.h
    nx/util/nxstrutil.h
    nx/util/nxhashes.h
    nx/util/nxbitutil.h
    nx/util/nxbitarray.h
    nx/event/nxeventdata.h
    nx/event/nxeventlistener.h
    nx/event/nxeventmanager.h
    nx/io/nxio.h
    nx/io/nxiobase.h
    nx/io/nxiofile.h
    nx/io/nxiomemory.h
    nx/io/nxvirtualfs.h
    nx/io/nxfilemanager.h
    nx/io/nxsandboxfs.h
    nx/io/nxiomemorymappedfile.h
    nx/io/nxarchivefs.h
    nx/hdl/nxhdl.h
    nx/hdl/nxhdlmanager.h
    nx/util/nxspscqueue.h
    nx/util/nxmpmcqueue.h
    nx/util/nxpool.h
    nx/util/nxtlsharedptr.h
    nx/task/nxtaskexecutor.h
    nx/task/nxtaskmanager.h
    nx/task/nxtask.h
    nx/sys/nxinput.h
    nx/sys/nxinputmanager.h
    nx/sys/nxinputctx.h
    nx/sys/nxwindow.h
    nx/sys/nxsysevents.h
    nx/sys/nxsystem.h
    nx/sys/sdl/nxsdlinput.h
    nx/sys/sdl/nxsystemsdl.h
    nx/sys/sdl/nxwindowsdl.h
    nx/sys/android/nxandroidinput.h
    nx/sys/android/nxsystemandroid.h
    nx/sys/android/nxwindowandroid.h
    nx/sys/nxapp.h
    nx/nx3d.h
    nx/ogl/core44/flextGL.h
    nx/ogl/nxoglinternal.h
    nx/ogl/nxogl.h
    nx/ogl/nxoglobj.h
    nx/ogl/nxoglprogram.h
    nx/ogl/nxogltexture.h
    nx/ogl/nxoglbuffer.h
    nx/ogl/nxgpuinterfaceogl.h
    nx/ogl/nxoglshaderinput.h
    nx/gpu/nxgpubuffermanagerinterface.h
    nx/gpu/nxgpubuffermanagersimple.h
    nx/gpu/nxgpu.h
    nx/gpu/nxgpuinterface.h
    nx/gpu/nxgpuprogramsource.h
    nx/gpu/nxgputexture.h
    nx/gpu/nxgpubuffer.h
    nx/gpu/nxgpushaderinput.h
    nx/gpu/nxgpumesh.h
    nx/gpu/nxgpusubmesh.h
    nx/media/nximage.h
    nx/media/nx3dmodel.h
    nx/math/nx3dmath.h
    nx/math/nxtransform.h
    nx/math/nxboundingvolumes.h
    nx/math/nxplane.h
    nx/math/nxray.h
    nx/math/nxintersect.h
    nx/scene/nxscenebase.h
    nx/scene/nxentitybase.h
    nx/resource/nxresourcemanager.h
    nx/resource/nxresource.h
    nx/resource/nximageresource.h
    nx/resource/nxgputextureresource.h
    nx/resource/nx3dmodelresource.h
    nx/resource/nxgpuprogramresource.h
    nx/resource/nxgpumeshresource.h
)


set(PROJ_SRC
    nx/allocator/nxmemory.cpp
    nx/allocator/nxalloctracker.cpp
    nx/log/nxlog.cpp
    nx/os/nxthread.cpp
    nx/os/sdl2/nxthreadsdl.cpp
    nx/os/sdl2/nxlocksdl.cpp
    nx/os/posix/nxthreadposix.cpp
    nx/os/posix/nxlockposix.cpp
    nx/os/nxlock.cpp
    nx/os/nxpath.cpp
    nx/os/nxsharedobj.cpp
    nx/os/nxargs.cpp
    nx/util/nxstrutil.cpp
    nx/util/nxhashes.cpp
    nx/util/nxbitutil.cpp
    nx/util/nxtime.cpp
    nx/event/nxeventmanager.cpp
    nx/io/nxiofile.cpp
    nx/io/nxiomemory.cpp
    nx/io/nxfilemanager.cpp
    nx/io/nxsandboxfs.cpp
    nx/io/nxiomemorymappedfile.cpp
    nx/io/nxarchivefs.cpp
    nx/hdl/nxhdlmanager.cpp
    nx/task/nxtaskexecutor.cpp
    nx/task/nxtaskmanager.cpp
    nx/task/nxtask.cpp
    nx/sys/nxinput.cpp
    nx/sys/nxinputmanager.cpp
    nx/sys/nxwindow.cpp
    nx/sys/nxsysevents.cpp
    nx/sys/nxsystem.cpp
    nx/sys/sdl/nxsdlinput.cpp
    nx/sys/sdl/nxsystemsdl.cpp
    nx/sys/sdl/nxwindowsdl.cpp
    nx/sys/android/nxandroidinput.cpp
    nx/sys/android/nxsystemandroid.cpp
    nx/sys/android/nxwindowandroid.cpp
    nx/sys/nxapp.cpp
    nx/nx3d.cpp
    nx/ogl/core44/flextGL.c
    nx/ogl/nxogl.cpp
    nx/ogl/nxoglprogram.cpp
    nx/ogl/nxogltexture.cpp
    nx/ogl/nxoglbuffer.cpp
    nx/ogl/nxgpuinterfaceogl.cpp
    nx/ogl/nxoglshaderinput.cpp
    nx/gpu/nxgpuprogramsource.cpp
    nx/gpu/nxgputexture.cpp
    nx/gpu/nxgpubuffer.cpp
    nx/gpu/nxgpubuffermanagersimple.cpp
    nx/gpu/nxgpushaderinput.cpp
    nx/gpu/nxgpumesh.cpp
    nx/gpu/nxgpusubmesh.cpp
    nx/media/nximage.cpp
    nx/media/nx3dmodel.cpp
    nx/math/nxtransform.cpp
    nx/math/nxplane.cpp
    nx/math/nxboundingvolumes.cpp
    nx/math/nxintersect.cpp
    nx/scene/nxscenebase.cpp
    nx/resource/nxresourcemanager.cpp
    nx/resource/nxresource.cpp
    nx/resource/nximageresource.cpp
    nx/resource/nxgputextureresource.cpp
    nx/resource/nx3dmodelresource.cpp
    nx/resource/nxgpuprogramresource.cpp
    nx/resource/nxgpumeshresource.cpp
)

if(ANDROID)
set(PROJ_SRC ${PROJ_SRC} nx/sys/android/android_native_app_glue.c)
endif()

add_library(${PROJ_NAME} STATIC  ${PROJ_HDR} ${PROJ_SRC})
add_dependencies(${PROJ_NAME} YAAF glm)

# compile idls`
NX_COMPILE_FLATBUFFER_IDL(${PROJ_NAME} ${CMAKE_CURRENT_BINARY_DIR}/genidl ${CMAKE_CURRENT_SOURCE_DIR}/idl PROJ_IDL)

# generate cmake config file
configure_file("cmake/nxconfig.cmake.in" "${NX_CONFIG_FILE_CMAKE}")

# install targets
install(TARGETS ${PROJ_NAME} DESTINATION ${NX_HOST_PREFIX}/lib EXPORT ${PROJ_NAME}_export)
foreach(HDR ${PROJ_HDR})
    get_filename_component(${HDR}_PATH ${HDR} PATH)
    install(FILES ${HDR} DESTINATION ${NX_HOST_PREFIX}/include/${${HDR}_PATH})
endforeach()
install(FILES ${NX_CONFIG_FILE} DESTINATION ${NX_HOST_PREFIX}/include/nx)
install(EXPORT ${PROJ_NAME}_export DESTINATION ${NX_HOST_PREFIX}/cmake-export)
install(FILES ${NX_CONFIG_FILE_CMAKE} DESTINATION ${NX_HOST_PREFIX}/cmake-export)
