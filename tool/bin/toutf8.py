# -*- coding: utf-8 -*-

import sys
import os

USAGE = """usage:
python toutf8.py path [-r][-B|b]
  path    path can be a directory or a file.
  -r      replace eol in sub directory rescusively.
  -b      with bom, this is default.
  -B      without bom.
"""

UTF8_BOM = "\xef\xbb\xbf"

FILE_IGNORE = set((
	".git",
	".svn",
	"third_party",
))
FILE_EXTS = set( (
	".h",
	".hpp",
	".ipp",
	".cpp",
	".c",
	".inl",
	".lua",
))

def get_file_ext(path):
	if len(path) == 0: return path

	pos = path.find(".")
	if pos < 0: return ""
	return path[ pos: ]

def usage(): print USAGE

def try_decode(source, encodings):
	for encoding in encodings:
		try:
			return source.decode(encoding)
		except:
			pass
	return None

def replace_in_file(fpath, withBom):
	handle = open(fpath, "rb")
	source = handle.read()
	handle.close()

	existBom = len(source) >= 3 and source[:3] == UTF8_BOM
	if existBom:
		source = source[3:]

	content = try_decode(source, ("utf8", "gbk", "cp1251") )
	if content is None:
		print "error: failed to decode. ", fpath
		return False

	content = content.encode("utf8")
	if existBom == withBom and content == source:
		return False
	
	#print "replaced: ", fpath

	handle = open(fpath, "wb")
	if withBom:
		handle.write(UTF8_BOM)

	handle.write(content)
	handle.close()

	return True

def replace_in_path(path, rescu, withBom):
	path = path.replace('\\', '/')
	if path[-1] != '/': path += '/'

	total = 0
	replaced = 0

	files = os.listdir(path)
	for fname in files:
		if fname in FILE_IGNORE: continue

		fname_ext = get_file_ext(fname)
		fname = path + fname
		
		if rescu and os.path.isdir(fname):
			t, r = replace_in_path(fname, rescu, withBom)
			total += t
			replaced += r

		elif fname_ext in FILE_EXTS:
			if replace_in_file(fname, withBom): replaced += 1
			total += 1
	return (total, replaced)

def main():

	num_arg = len(sys.argv)

	if num_arg < 2: return usage()

	path = None
	rescu = False
	withBom = True

	i = 1
	while i < num_arg:
		cmd = sys.argv[i]
		if cmd == "-r":
			rescu = True

		elif cmd == "-B":
			withBom = False

		elif cmd == "-b":
			withBom = True

		elif path is None:
			path = cmd

		else:
			print "invalid cmd ", cmd
			return usage()

		i += 1


	if path is None or not os.path.exists(path):
		print "invalid path"
		return usage()

	total, replaced = replace_in_path(path, rescu, withBom)
	print "\nPath: ", path
	print "Total file %d, Replaced %d" % (total, replaced)

	return

if __name__ == "__main__":
	main()

