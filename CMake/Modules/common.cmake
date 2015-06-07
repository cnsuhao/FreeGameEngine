
message("CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})
message("CMAKE_CONFIGURATION_TYPES " ${CMAKE_CONFIGURATION_TYPES})

if (IOS)
	message("platform: ios")
	add_definitions(-DIOS=1)
elseif (ANDROID)
	message("platform: android")
	add_definitions(-DANDROID=1)
elseif(WIN32)
	message("platform: win32")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
	message("platform: apple")
	add_definitions(-DAPPLE=1)
else()
	message("platform: unknow")
endif ()


set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -D_RELEASE")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -D_RELEASE")

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")

if (ANDROID OR IOS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fexceptions -std=c++11 -fsigned-char")
endif ()

set (ARG_GLOB_CPP_PATTERNS *.c *.cpp)
set (ARG_GLOB_H_PATTERNS *.h *.hpp *.ipp)

if(IOS)
	list(APPEND ARG_GLOB_CPP_PATTERNS *.mm)
endif()

macro (collect_source_files)

    file (GLOB CPP_FILES ${ARG_GLOB_CPP_PATTERNS})
    file (GLOB H_FILES ${ARG_GLOB_H_PATTERNS})
    
    set (SOURCE_FILES ${H_FILES} ${CPP_FILES})

endmacro()


macro (set_source_files_to_parent)
    get_filename_component (DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    set (${DIR_NAME}_CPP_FILES ${CPP_FILES} PARENT_SCOPE)
    set (${DIR_NAME}_H_FILES ${H_FILES} PARENT_SCOPE)
endmacro()


macro (define_source_files)
    
    collect_source_files()
    set_source_files_to_parent()
    
endmacro ()


macro (setup_target)

    if(INCLUDE_DIRECTORIES)
        include_directories (${INCLUDE_DIRECTORIES})
    endif()

    if(LINK_DIRECTORIES)
        message("link dir" ${LINK_DIRECTORIES})
        link_directories(${LINK_DIRECTORIES})
    endif()

    if(LINK_LIBRARIES)
        target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})
    endif()

    if (XCODE)

        set_property_xcode(${TARGET_NAME} CLANG_CXX_LANGUAGE_STANDARD "c++11")
        set_property_xcode(${TARGET_NAME} CLANG_CXX_LIBRARY "libc++")
        set_property_xcode(${TARGET_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS "YES")

    endif ()

endmacro ()


macro (setup_library)
    add_library (${TARGET_NAME} ${SOURCE_FILES})
    setup_target ()

    set(ALL_LIBRARY_TARGETS ${ALL_LIBRARY_TARGETS} ${TARGET_NAME} PARENT_SCOPE)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
endmacro ()


macro (setup_executable)
    
    add_executable(${TARGET_NAME} ${EXECUTEABLE_PARAMTERS} ${SOURCE_FILES})
    setup_target ()

    if (IOS)
        set_target_properties (${TARGET_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    endif ()

    if(EXE_OUTPUT_DIR)
        set(EXECUTABLE_OUTPUT_PATH ${EXE_OUTPUT_DIR})
    else ()
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    endif()

endmacro ()

macro (setup_dynamic)
    
	add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})
    setup_target ()

    if (IOS)
        set_target_properties (${TARGET_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    endif ()

    if(EXE_OUTPUT_DIR)
        set(EXECUTABLE_OUTPUT_PATH ${EXE_OUTPUT_DIR})
    else ()
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    endif()

endmacro ()

# Macro for setting up linker flags for Mac OS X desktop build
macro (setup_macosx_linker_flags LINKER_FLAGS)
    set (${LINKER_FLAGS} "${${LINKER_FLAGS}} -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices")
endmacro ()

# Macro for setting up linker flags for IOS build
macro (setup_ios_linker_flags LINKER_FLAGS)
    set (${LINKER_FLAGS} "${${LINKER_FLAGS}} -framework AudioToolbox -framework CoreAudio -framework CoreGraphics -framework Foundation -framework OpenGLES -framework QuartzCore -framework UIKit")
endmacro ()

macro (set_property_xcode TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_property_xcode)

