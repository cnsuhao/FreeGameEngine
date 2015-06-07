
set(TARGET_NAME luasocket)

set (SOURCE_FILES 
    src/auxiliar.c
    src/auxiliar.h
    src/buffer.c
    src/buffer.h
    src/except.c
    src/except.h
    src/inet.c
    src/inet.h
    src/io.c
    src/io.h
    src/luasocket.c
    src/luasocket.h
    src/mime.c
    src/mime.h
    src/options.c
    src/options.h
    src/select.c
    src/select.h
    src/tcp.c
    src/tcp.h
    src/timeout.c
    src/timeout.h
    src/udp.c
    src/udp.h
)

if (WIN32)
    list(APPEND SOURCE_FILES
        src/wsocket.c
        src/wsocket.h
    )
else()
    list(APPEND SOURCE_FILES
        src/usocket.c
        src/usocket.h
    )
endif()

set (INCLUDE_DIRECTORIES ../lua)

setup_library()
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "third_party")
