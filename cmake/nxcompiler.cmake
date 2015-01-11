#------------------------------------------------------------------------------
# NX : Compiler settings
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Compiler Check features
#------------------------------------------------------------------------------
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if(NOT MSVC)
    set(NX_C_WARNING_FLAGS "-Wall -Werror -Wextra -Winit-self -Wuninitialized -Wmissing-declarations -pedantic -fvisibility=hidden")
    set(NX_CXX_WARNING_FLAGS "-Woverloaded-virtual")
    set(NX_C_COMPILE_FLAGS "-std=c99")
    set(NX_CXX_COMPILE_FLAGS "-std=c++11 -fno-exceptions -fno-rtti")
    set(NX_C_COMPILE_FLAGS_DEBUG "-g -O0 -fno-inline -DNX_DEBUG")
    set(NX_CXX_COMPILE_FLAGS_DEBUG "-g -O0 -fno-inline -DNX_DEBUG")
    set(NX_C_COMPILE_FLAGS_RELEASE "-O3 -g")
    set(NX_CXX_COMPILE_FLAGS_RELEASE "-O3 -g")

    # test for SSE
    if(NOT DEFINED ANDROID OR DEFINED ANDROID-X86)
        check_cxx_compiler_flag("-msse2" NX_HAVE_SSE2)
        check_cxx_compiler_flag("-msse4" NX_HAVE_SSE4)
        check_cxx_compiler_flag("-msse4.1" NX_HAVE_SSE41)
        check_cxx_compiler_flag("-msse4.2" NX_HAVE_SSE42)

        if(${NX_HAVE_SSE2})
             set(NX_C_COMPILE_FLAGS "${NX_C_COMPILE_FLAGS} -msse2")
        endif()

        if(${NX_HAVE_SSE4})
            set(NX_C_COMPILE_FLAGS "${NX_C_COMPILE_FLAGS} -msse4")
        endif()

        if(${NX_HAVE_SSE41})
            set(NX_C_COMPILE_FLAGS "${NX_C_COMPILE_FLAGS} -msse4.1")
        endif()

        if(${NX_HAVE_SSE42})
            set(NX_C_COMPILE_FLAGS "${NX_C_COMPILE_FLAGS} -msse4.2")
        endif()
    endif()

    if(ANDROID)
        set(NX_C_COMPILE_FLAGS "${NX_C_COMPILE_FLAGS} ${ANDROID_C_FLAGS}")
        set(NX_CXX_COMPILE_FLAGS "${NX_CXX_COMPILE_FLAGS} ${ANDROID_CXX_FLAGS}")

        if(ANDROID-ARM)
            set(NX_HAVE_NEON 1)
        endif()
    endif()

else()
    set(NX_C_WARNING_FLAGS )
    set(NX_CXX_WARNING_FLAGS )
    set(NX_C_COMPILE_FLAGS )
    set(NX_CXX_COMPILE_FLAGS )
    set(NX_C_COMPILE_FLAGS_DEBUG "/DNX_DEBUG")
    set(NX_CXX_COMPILE_FLAGS_DEBUG "/DNX_DEBUG")
    set(NX_C_COMPILE_FLAGS_RELEASE )
    set(NX_CXX_COMPILE_FLAGS_RELEASE )
endif()

set(CMAKE_C_FLAGS "${NX_C_WARNING_FLAGS} ${NX_C_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${NX_C_WARNING_FLAGS} ${NX_CXX_WARNING_FLAGS} ${NX_CXX_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${NX_C_COMPILE_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${NX_CXX_COMPILE_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DEBUG "${NX_C_COMPILE_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG "${NX_CXX_COMPILE_FLAGS_DEBUG}")


