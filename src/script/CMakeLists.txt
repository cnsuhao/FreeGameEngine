# Define target name
set (TARGET_NAME script)

# Define source files
file (GLOB SCRIPTS_FILES *.h *.cpp)
file (GLOB GLUE_FILES luamid/*.cpp)
set (SOURCE_FILES ${SCRIPTS_FILES} ${GLUE_FILES})

# Define dependency libs
set (INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
    ${THIRD_PARTY_DIR}
	${THIRD_PARTY_DIR}/lua
	${THIRD_PARTY_DIR}/toluaplus/include
)

source_group ("scripts\\" FILES ${SCRIPTS_FILES})
source_group ("glue\\" FILES ${GLUE_FILES})

add_definitions(
	-DMYGUI_STATIC
	-DMYGUI_USE_FREETYPE
	-DCODE_INLINE
)

# Setup target
setup_library ()
