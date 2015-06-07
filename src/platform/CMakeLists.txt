

set(TARGET_NAME platform)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

add_definitions(-DCODE_INLINE)

if (IOS)
	set(SUB_DIR ios)
elseif (ANDROID)
	set(SUB_DIR android)
elseif (WIN32)
	set(SUB_DIR win32)
else ()
	set(SUB_DIR unix)
endif()

message(STATUS "platform is " ${SUB_DIR})

file (GLOB SOURCE_FILES *.h *.cpp
	${SUB_DIR}/*.h ${SUB_DIR}/*.cpp)

setup_library()
