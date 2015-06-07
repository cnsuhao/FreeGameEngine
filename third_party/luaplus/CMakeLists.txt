# Define target name
set (TARGET_NAME luaplus)

# Define source files
set (SOURCE_FILES 
	LuaAutoBlock.h
	LuaCall.h
	LuaFunction.h
	LuaHelper.h
	LuaHelper_Object.h
	LuaHelper_StackObject.h
	LuaLink.h
	LuaObject.cpp
	LuaObject.h
	LuaObject.inl
	LuaPlus.cpp
	LuaPlus.h
	LuaPlusAddons.c
	LuaPlusCD.h
	LuaPlusInternal.h
	LuaPlus_Libs.cpp
	LuaStackObject.h
	LuaStackObject.inl
	LuaStackTableIterator.h
	LuaState.cpp
	LuaState.h
	LuaState.inl
	LuaStateOutFile.cpp
	LuaStateOutFile.h
	LuaState_DumpObject.cpp
	LuaTableIterator.cpp
	LuaTableIterator.h
	lwstrlib.c
	popen.c
	lpeg/lpeg.h
	lpeg/lpeg.c
	)

# Define dependency libs
set (INCLUDE_DIRECTORIES ${THIRD_PARTY_DIR}/lua)

# Setup target
setup_library ()

