#------------------------------------------------------------------------------
# NX : Cross Compiling settings
#------------------------------------------------------------------------------

#set NX_HOST_PREFIX if we are not cross compiling
if(NOT DEFINED NX_CROSS_COMPILE)
    #if( CMAKE_HOST_APPLE )
    #    set(NX_HOST_PREFIX "darwin-x86" )
    #elseif(CMAKE_HOST_WIN32 )
    #    set(NX_HOST_PREFIX "windows" )
    #elseif(CMAKE_HOST_UNIX)
    #    set(NX_HOST_PREFIX "linux-x86" )
    #else()
    #    message(SEND_ERROR "Unknown host system detected")
    #endif()
    set(NX_HOST_PREFIX "host")
endif()
