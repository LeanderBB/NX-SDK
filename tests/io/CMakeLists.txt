#------------------------------------------------------------------------------
# small io test
#------------------------------------------------------------------------------

include_directories(${NXCORE_INCLUDE_DIRS})
add_executable(nxiotest nxiotest.cpp)
target_link_libraries(nxiotest ${NXCORE_LIBRARIES})

add_executable(nxarchivetest nxarchivetest.cpp)
target_link_libraries(nxarchivetest ${NXCORE_LIBRARIES})

