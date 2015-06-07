# Define target name
set (TARGET_NAME glew)

# Define source files
file (GLOB C_FILES *.c)
file (GLOB H_FILES *.h)
set (SOURCE_FILES ${C_FILES} ${H_FILES})

# Define dependency libs
set (INCLUDE_DIRECTORIES .)

# Setup target
setup_library ()

add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "third_party")
