#------------------------------------------------------------------------------
# small args test
#------------------------------------------------------------------------------

include_directories(${NXCORE_INCLUDE_DIRS})
add_executable(nxargstest nxargstest.cpp)
target_link_libraries(nxargstest ${NXCORE_LIBRARIES})
