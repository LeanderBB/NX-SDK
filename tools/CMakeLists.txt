#------------------------------------------------------------------------------
# NX Tools
#------------------------------------------------------------------------------

set(NXTOOLS_INCLUDE_DIRS
    ${NXCORE_INCLUDE_DIRS}
)
set(NXTOOLS_LIBRARIES
    ${NXCORE_LIBRARIES}
)

add_subdirectory(nxprogrambuilder)
add_subdirectory(nxktxtonximg)
add_subdirectory(nximginfo)
add_subdirectory(nximgviewer)
add_subdirectory(nx3dconverter)
add_subdirectory(nx3dinfo)
add_subdirectory(nxbuildimg)
add_subdirectory(nxastctonximg)
add_subdirectory(nximgconverter)
