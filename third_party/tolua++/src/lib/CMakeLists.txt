# Define target name
set (TARGET_NAME tolualib)

# Define source files
file (GLOB SOURCE_FILES *.c *.h)

set (INCLUDE_DIRECTORIES ../../include ../../../lua)

# Setup target
setup_library ()
