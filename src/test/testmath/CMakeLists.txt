
set(TARGET_NAME testmath)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

file (GLOB SOURCE_FILES *.h *.cpp)

add_definitions(-DCODE_INLINE)

set (LINK_LIBRARIES util math cstdmf)

# Setup target
setup_executable ()
