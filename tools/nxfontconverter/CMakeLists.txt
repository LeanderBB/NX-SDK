#------------------------------------------------------------------------------
# Convert fonts to engine readble format  with the help of FreeType and stb_image
# stb_image: https://github.com/nothings/stb
#------------------------------------------------------------------------------

set(PROJ_NAME nxfontconverter)


find_package(Freetype REQUIRED)


include_directories(${NXTOOLS_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})


set(PROJ_SRC
    src/main.cpp
)

set(PROJ_HDR )

add_executable(${PROJ_NAME} ${PROJ_SRC})

target_link_libraries(${PROJ_NAME} ${NXTOOLS_LIBRARIES} ${FREETYPE_LIBRARIES})

install(TARGETS ${PROJ_NAME} DESTINATION bin)
