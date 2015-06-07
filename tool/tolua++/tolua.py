# -*- coding: utf-8 -*-

import os
import sys


#开头不含 '.'
def get_file_ext(name):
	pos = name.find('.')
	if pos < 0: return None

	return name[ pos+1: ]

#末尾不含 '.'
def remove_file_ext(name):
	pos = name.find('.')
	if pos < 0: return None

	return name[ :pos ]


def get_file_path(name):
	if len(name) == 0: return name

	name = format_file_name(name)
	pos = name.rfind('/')
	if pos < 0: return ""

	return name[ :pos+1 ]


def get_file_name(name):
	if len(name) == 0: return name

	name = format_file_name(name)
	pos = name.rfind('/')
	if pos < 0: return name

	return name[ pos+1: ]


#末尾含 '/'
def format_path_name(name):
	if len(name) == 0: return name

	name = name.replace('\\', '/')
	if name[-1] != '/': name += '/'

	return name


#末尾不含 '/'
def format_file_name(name):
	if len(name) == 0: return name

	name = name.replace('\\', '/')
	if name[-1] == '/': name = name[:-1]

	return name


def if_file_ext_match(name, filters=None):
	if filters is None: return True

	ext = get_file_ext(name)
	if ext is None: return False

	return ext in filters

def collect_files(path, filters, rescusive=False):
	ret = []

	files = os.listdir(path)
	for name in files:
		full_path = path + name
		if os.path.isdir(full_path):
			if not rescusive: continue

			ret.extend(collect_files(full_path + "/", filters, rescusive))

		else:
			if not if_file_ext_match(name, filters): continue

			ret.append(full_path)

	return ret

##################################################
## 
##################################################
USAGE = """usage:
python tolua.py src_path dst_path"""

def main():
	if len(sys.argv) < 2:
		print USAGE
		return

	app_path = os.path.abspath(sys.argv[0])
	src_path = format_path_name(sys.argv[1])
	dst_path = format_path_name(sys.argv[2])

	app_path = get_file_path(app_path) + "toluaapp"
	cur_path = format_path_name(os.getcwd())
	dst_path = cur_path + dst_path
	src_path = cur_path + src_path

	os.chdir(src_path)

	files = collect_files("./", ("pk", ))
	if len(files) == 0:
		print "warn: doens't find any files in ", src_path
		return

	if not os.path.isdir(dst_path):
		os.mkdir(dst_path)

	for file_name in files:
		dst_name = get_file_name(file_name)
		dst_name = remove_file_ext(dst_name)
		dst_file = dst_path + dst_name + ".cpp"

		cmd_line = "%s -n %s -o %s %s" % (app_path, dst_name, dst_file, file_name)
		print cmd_line
		os.system(cmd_line)


if __name__ == "__main__":
	main()
