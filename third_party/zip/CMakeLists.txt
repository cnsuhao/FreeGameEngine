
set(TARGET_NAME zip)


set (SOURCE_FILES
	compress.c
	adler32.c
	compress.c
	crc32.c
	crc32.h
	deflate.c
	deflate.h
	gzio.c
	infback.c
	inffast.h
	inffast.c
	inffixed.h
	inflate.c
	inflate.h
	inftrees.c
	inftrees.h
	trees.c
	trees.h
	uncompr.c
	zconf.h
	zconf.in.h
	zlib.h
	zutil.c
	zutil.h
)

add_definitions(-DCODE_INLINE)
setup_library()
