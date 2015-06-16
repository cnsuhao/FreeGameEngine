# Define target name
set (TARGET_NAME script)

# Define source files
file (GLOB SCRIPTS_FILES *.h *.cpp *.inl)
file (GLOB EXPORTS_FILES export/*.cpp)
file (GLOB AUTO_FILES auto/*.hpp auto/*.cpp)
set (SOURCE_FILES ${SCRIPTS_FILES} ${EXPORTS_FILES} ${AUTO_FILES})

# Define dependency libs
set (INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/src
    ${THIRD_PARTY_DIR}
	${THIRD_PARTY_DIR}/lua
	${THIRD_PARTY_DIR}/tolua++/include
)

source_group ("helper\\" FILES ${SCRIPTS_FILES})
source_group ("manual\\" FILES ${EXPORTS_FILES})
source_group ("auto\\" FILES ${AUTO_FILES})

add_definitions(
	-DCODE_INLINE
)

# Setup target
setup_library ()
