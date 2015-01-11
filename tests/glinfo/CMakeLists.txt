#------------------------------------------------------------------------------
# glinfo - print opengl info and exit
#------------------------------------------------------------------------------

include_directories(${NXCORE_INCLUDE_DIRS})
add_executable(nxglinfo nxglinfo.cpp)
target_link_libraries(nxglinfo ${NXCORE_LIBRARIES})
