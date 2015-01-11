#------------------------------------------------------------------------------
# Input test - print out all input events
#------------------------------------------------------------------------------



include_directories(${NXCORE_INCLUDE_DIRS})
add_executable(nxtestinput nxtestinput.cpp)
target_link_libraries(nxtestinput ${NXCORE_LIBRARIES})
