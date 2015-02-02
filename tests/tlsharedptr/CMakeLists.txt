#------------------------------------------------------------------------------
# small test for tlsharedptr
#------------------------------------------------------------------------------

include_directories(${NXCORE_INCLUDE_DIRS})
add_executable(nxtesttlsharedptr nxtesttlsharedptr.cpp)
target_link_libraries(nxtesttlsharedptr ${NXCORE_LIBRARIES})
