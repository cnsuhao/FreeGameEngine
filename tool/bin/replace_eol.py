# -*- coding: utf-8 -*-

import sys
import os

USAGE = """usage:
python replace_eol.py path [-r] [-eol lf|cr|crlf] [-ext extension]
  path    path can be a directory or a file.
  -r      replace eol in sub directory rescusively.
  -eol    the dest line ending. can be lf, cr or crlf, default is lf.
  -ext    the extension of files will be processed. eg: "txt;cpp".
          if "-ext" doesn't specified, the default extension will be used.
"""

DEFAULT_EXTS = set( (
	".h",
	".hpp",
	".ipp",
	".cpp",
	".c",
	".inl",
	".lua",
	".pk",
	".glsl",
	".fx",
	".cmake", 
	".txt",
	".md",
	".gitignore",
	".gitattributes",
	".gitmodules",
	".xml",
	".json",
	".py",
	".tab",
	".sht",
	".mtl",
	".list",
	".html",
	".layout",
	".bat",
))
FILE_IGNORE = set((
	".git",
	".svn",
))

EOL_MAP = {
	"lf": "\n",
	"cr": "\r",
	"crlf": "\r\n",
}

def usage(): print USAGE

def get_file_ext(path):
	if len(path) == 0: return path

	pos = path.rfind(".")
	if pos < 0: return ""
	return path[ pos: ]

def replace_in_file(fpath, eol):
	handle = open(fpath, "rb")
	source = handle.read()
	handle.close()

	content = source.replace("\r\n", "\n")
	content = content.replace("\r", "\n")
	if eol != "\n": content = content.replace("\n", eol)

	if content == source: return False

	print "replaced: ", fpath

	handle = open(fpath, "wb")
	handle.write(content)
	handle.close()

	return True


def replace_in_path(path, eol, rescu, exts):
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
			t, r = replace_in_path(fname, eol, rescu, exts)
			total += t
			replaced += r

		elif fname_ext in exts:
			if replace_in_file(fname, eol): replaced += 1
			total += 1

	return (total, replaced)


def replace_line_ending(path, eol, rescu, exts):
	if os.path.isdir(path):
		total, replaced = replace_in_path(path, eol, rescu, exts)

		print "\nPath: ", path
		print "Total file %d, Replaced %d" % (total, replaced)
	else:
		replace_in_file(path, eol)


def main():

	num_arg = len(sys.argv)

	if num_arg < 2: return usage()

	path = None
	rescu = False
	eol = "\n"
	exts = DEFAULT_EXTS

	i = 0
	while i < num_arg:
		cmd = sys.argv[i]
		if cmd == "-r":
			rescu = True

		elif cmd == "-eol":
			if i + 1 >= num_arg: return usage()

			i += 1
			cmd = sys.argv[i]
			eol = EOL_MAP.get(cmd.lower())
			if eol is None:
				print "invalid eol", cmd
				return usage()

		elif cmd == "-ext":
			if i + 1 >= num_arg: return usage()

			i += 1
			cmd = sys.argv[i]
			exts = cmd.split(";")
			exts = set( ["." + x for x in exts] )

		elif cmd[0] == "-":
			print "invalid cmd ", cmd
			return usage()

		else:
			path = cmd

		i += 1


	if path is None or not os.path.exists(path):
		print "invalid path"
		return usage()

	replace_line_ending(path, eol, rescu, exts)
	return

if __name__ == "__main__":
	main()

