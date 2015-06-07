# Define target name
set (TARGET_NAME toluaapp)

# Define source files
set (SOURCE_FILES 
	tolua.c
	toluabind.h
	toluabind.c
)

set (INCLUDE_DIRECTORIES
	../../include
	../../../lua
)

#add_definitions(-DTOLUA_SCRIPT_RUN)

set (LINK_LIBRARIES lua luaplus tolualib)

# Setup target
set(EXE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/tool/toluaplus)
setup_executable ()
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "third_party")
