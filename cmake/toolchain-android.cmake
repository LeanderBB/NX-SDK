#------------------------------------------------------------------------------
# Stand Alone Android Toolchain
#
# Current support limited to armv7a with neon
#------------------------------------------------------------------------------

# CMake setup variables
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(UNIX 1)
set(ANDROID 1)
set(ANDROID-ARM 1)

INCLUDE(CMakeForceCompiler)

# build only the nx library, skip the rest
set(NX_CROSS_COMPILE 1)


#set exec suffix for the platform
if(CMAKE_HOST_WIN32)
set(TOOL_OS_SUFFIX .exe)
endif()


if(NOT DEFINED ANDROID_TOOLCHAIN_DIR)
    if(DEFINED ENV{ANDROID_TOOLCHAIN_DIR})
        set(ANDROID_TOOLCHAIN_DIR $ENV{ANDROID_TOOLCHAIN_DIR})
    else()
        message(SEND_ERROR "Please define ANDROID_TOOLCHAIN_DIR via cmake or as an env variable")
    endif()
endif()

if(DEFINED ANDROID_TOOLCHAIN_DIR AND NOT DEFINED ENV{ANDROID_TOOLCHAIN_DIR})
    set(ENV{ANDROID_TOOLCHAIN_DIR} ${ANDROID_TOOLCHAIN_DIR})
endif()

if(NOT DEFINED ANDROID_SDK_DIR)
    if(DEFINED ENV{ANDROID_SDK_DIR})
        set(ANDROID_SDK_DIR $ENV{ANDROID_SDK_DIR})
    else()
        message(SEND_ERROR "Please define ANDROID_SDK_DIR via cmake or as an env variable")
    endif()

    set(ENV{ANDROID_HOME} ${ANDROID_SDK_DIR})
endif()

if(DEFINED ANDROID_SDK_DIR AND NOT DEFINED ENV{ANDROID_SDK_DIR})
    set(ENV{ANDROID_SDK_DIR} ${ANDROID_SDK_DIR})
endif()

if(NOT DEFINED ANDROID_NDK_DIR)
    if(DEFINED ENV{ANDROID_NDK_DIR})
        set(ANDROID_NDK_DIR $ENV{ANDROID_NDK_DIR})
    else()
        message(SEND_ERROR "Please define ANDROID_NDK_DIR via cmake or as an env variable")
    endif()
endif()

if(DEFINED ANDROID_NDK_DIR AND NOT DEFINED ENV{ANDROID_NDK_DIR})
    set(ENV{ANDROID_NDK_DIR} ${ANDROID_NDK_DIR})
endif()

set(ANDROID_ABI_ARM arm-linux-androideabi)
set(ANDROID_ABI ${ANDROID_ABI_ARM})

set(NX_HOST_PREFIX ${ANDROID_ABI})

# use clang or gcc
#if(NOT DEFINED ANDROID_TOOLCHAIN_CLANG OR NOT DEFINED ENV{ANDROID_TOOLCHAIN_CLANG})
#set(ANDROID_CC "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-gcc${TOOL_OS_SUFFIX}")
#set(ANDROID_CXX "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-g++${TOOL_OS_SUFFIX}")
#set(CMAKE_ASM_COMPILER "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-gcc${TOOL_OS_SUFFIX}" CACHE PATH "Assembler" )
#else()
set(ANDROID_CC "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-clang${TOOL_OS_SUFFIX}")
set(ANDROID_CXX "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-clang++${TOOL_OS_SUFFIX}")
set(CMAKE_ASM_COMPILER "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-clang${TOOL_OS_SUFFIX}" CACHE PATH "Assembler" )
#endif()

# specify the cross compiler
SET(CMAKE_C_COMPILER   "${ANDROID_CC}")
SET(CMAKE_CXX_COMPILER "${ANDROID_CXX}")


#setup other tools
set( CMAKE_STRIP        "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-strip${TOOL_OS_SUFFIX}"   CACHE PATH "strip" )
set( CMAKE_AR           "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-ar${TOOL_OS_SUFFIX}"      CACHE PATH "archive" )
set( CMAKE_LINKER       "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-ld${TOOL_OS_SUFFIX}"      CACHE PATH "linker" )
set( CMAKE_NM           "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-nm${TOOL_OS_SUFFIX}"      CACHE PATH "nm" )
set( CMAKE_OBJCOPY      "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-objcopy${TOOL_OS_SUFFIX}" CACHE PATH "objcopy" )
set( CMAKE_OBJDUMP      "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-objdump${TOOL_OS_SUFFIX}" CACHE PATH "objdump" )
set( CMAKE_RANLIB       "${ANDROID_TOOLCHAIN_DIR}/bin/${ANDROID_ABI}-ranlib${TOOL_OS_SUFFIX}"  CACHE PATH "ranlib" )
set( CMAKE_TOOLCHAIN_PREFIX "${ANDROID_ABI}-" )


# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  ${ANDROID_TOOLCHAIN_DIR})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# compiler flags
# arm32 + NEON
set(ANDROID_C_FLAGS "-DANDROID -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=neon -fstrict-aliasing")
set(ANDROID_CXX_FLAGS "-DANDROID -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=neon -fstrict-aliasing")
set(ANDROID_LD_FLAGS "-march=armv7-a -Wl,--fix-cortex-a8")
set(ANDROID_LIBS android log)

set(CMAKE_C_FLAGS "${ANDROID_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${ANDROID_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ANDROID_LD_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ANDROID_LD_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ANDROID_LD_FLAGS} -Wl,--no-undefined")
