
set (TARGET_NAME client)

add_definitions(
	-DCODE_INLINE
)

file(GLOB SOURCE_FILES
	src/*.h
	src/*.cpp
)

set(INCLUDE_DIRECTORIES 
	${PROJECT_SOURCE_DIR}/src
	${THIRD_PARTY_DIR}
)

set(LINK_LIBRARIES
	util core math graphics platform script
	FreeType SDL luasocket luaplus tolualib lua stb zip
)

if (WIN32)
	add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

	list(APPEND SOURCE_FILES 
		proj.win32/game.rc
		proj.win32/resource.h
	)
	
	list(APPEND LINK_LIBRARIES glew winmm imm32 version opengl32 shlwapi Ws2_32)

	set(EXECUTEABLE_PARAMTERS WIN32)

elseif (APPLE)
	setup_macosx_linker_flags (CMAKE_EXE_LINKER_FLAGS)
	#list(APPEND LINK_LIBRARIES curl iconv)

elseif (ANDROID)

	message(STATUS "ANDROID_ABI: " ${ANDROID_ABI})
	message(STATUS "ANDROID_NATIVE_API_LEVEL: " ${ANDROID_NATIVE_API_LEVEL})
	message(STATUS "third party: " ${THIRD_PARTY_DIR})

	list(APPEND SOURCE_FILES proj.android/jni/client_jni.cpp)

	list(APPEND LINK_LIBRARIES
		GLESv1_CM GLESv2 log dl android z
	)

endif()


if (ANDROID)
	add_library (${TARGET_NAME} SHARED ${SOURCE_FILES})
	include_directories (${INCLUDE_DIRECTORIES})
	target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})

	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/game/client/proj.android/libs/${ANDROID_ABI})
	message(STATUS "output path: " ${LIBRARY_OUTPUT_PATH})

	#set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "client")

elseif(IOS)
	message(ERROR "currently doesn't support for ios.")

else ()

	set(EXE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/game/client/bin)
	setup_executable()
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)
    set_target_properties(${TARGET_NAME} PROPERTIES RELWITHDEBINFO_POSTFIX _h)
	set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "gameapp")

endif()
