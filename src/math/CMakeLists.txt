
set(TARGET_NAME math)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

file (GLOB SOURCE_FILES *.hpp *.ipp *.cpp)

add_definitions(-DCODE_INLINE)
setup_library()
