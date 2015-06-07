
#PROJECT(third_party)


set(SOURCES
	SDL FreeType stb zip
	lua luaplus tolua++/src/lib luasocket
)


if(WIN32)
    list(APPEND SOURCES glew tolua++/src/bin)

elseif(APPLE)
	list(APPEND SOURCES tolua++/src/bin)
endif()

foreach (TARGET_NAME ${SOURCES})
    add_subdirectory (${TARGET_NAME})
endforeach ()

foreach (TARGET_NAME SDL FreeType stb zip lua luaplus tolualib)
	set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "third_party")
endforeach()
