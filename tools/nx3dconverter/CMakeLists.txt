#------------------------------------------------------------------------------
# Convert 3d meshes to nx3d format with the help of ASSIMP
#------------------------------------------------------------------------------


set(PROJ_NAME nx3dconverter)

find_package(ASSIMP REQUIRED)




include_directories(${NXTOOLS_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})


set(PROJ_SRC
    src/main.cpp
    src/nxassimpconverter.cpp
    src/nx3dmodelbuilder.cpp
    src/nx3dmodelpostprocess.cpp
)

set(PROJ_HDR
    src/nx3dconverter.h
    src/nx3dmodelbuilder.h
    src/nx3dmodelpostprocess.h
)

add_executable(${PROJ_NAME} ${PROJ_SRC} ${PROJ_HDR})

target_link_libraries(${PROJ_NAME} ${NXTOOLS_LIBRARIES} ${ASSIMP_LIBRARIES})

install(TARGETS ${PROJ_NAME} DESTINATION bin)
