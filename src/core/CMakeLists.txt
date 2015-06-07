
set(TARGET_NAME core)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

file (GLOB SOURCE_FILES *.h *.cpp)

add_definitions(-DCODE_INLINE)
setup_library()
