
set(TARGET_NAME graphics)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

file (GLOB SOURCE_FILES *.h *.cpp)

add_definitions(
	-DCODE_INLINE
)

if(WIN32)
	add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)
endif()

setup_library()
