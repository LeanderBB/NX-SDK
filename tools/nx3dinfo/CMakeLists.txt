#------------------------------------------------------------------------------
# NXImage info printer
#------------------------------------------------------------------------------


set(PROJ_NAME nx3dinfo)


include_directories(${NXTOOLS_INCLUDE_DIRS})


set(PROJ_SRC
    src/main.cpp
)

set(PROJ_HDR )

add_executable(${PROJ_NAME} ${PROJ_SRC})

target_link_libraries(${PROJ_NAME} ${NXTOOLS_LIBRARIES})

install(TARGETS ${PROJ_NAME} DESTINATION bin)
