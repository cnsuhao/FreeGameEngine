#-*- coding: utf-8 -*-

import os
import sys

USAGE = """usage:
python shader_parser.py input_file [output_file]
output_file: same as input_file, if not assign.
"""

SHADER_MAGIC = "\xff\x6f\x73\x68"
SHADER_PWD = [
    148, 237, 80, 109, 185, 134, 86, 145,
    95, 211, 193, 16, 53, 91, 31, 166,
    246, 114, 140, 188, 47, 228, 202, 130,
    234, 131, 138, 73, 187, 215, 220, 99
]

def usage():
	print USAGE

def if_encryted(content):
	return len(content) >= 4 and content[:4] == SHADER_MAGIC

#################################################
###
#################################################
def encrypt(content):
	pwd_length = len(SHADER_PWD)

	ret = SHADER_MAGIC
	i = 0
	for c in content:
		ret += chr(ord(c) ^ SHADER_PWD[i])
		i = (i + 1) % pwd_length
	return ret

def do_encrypt(input_name, output_name):
	handle = open(input_name, "rb")
	if handle is None: return False
	
	content = handle.read()
	if not if_encryted(content):
		content = encrypt(content)

	handle = open(output_name, "wb")
	if handle is None: return False

	handle.write(content)
	return True

#################################################
###
#################################################
def decrypt(content):
	pwd_length = len(SHADER_PWD)

	ret = ""
	i = 0
	for i in xrange(len(content) - 4):
		c = content[i + 4]
		ret += chr(ord(c) ^ SHADER_PWD[i % pwd_length])
	return ret

def do_decrypt(input_name, output_name):
	handle = open(input_name, "rb")
	if handle is None: return False
	
	content = handle.read()
	if if_encryted(content):
		content = decrypt(content)

	handle = open(output_name, "wb")
	if handle is None: return False

	handle.write(content)
	return True

#################################################
###
#################################################
def encrypt_directory(path, extensions):
	total = 0
	succed = 0

	for root, dirs, files in os.walk(path):

		for fname in files:
			name, ext = os.path.splitext(fname)
			if ext not in extensions: continue

			total += 1
			path = os.path.join(root, fname)
			if do_encrypt(path, path): succed += 1

	print "total %d, succed %d" % (total, succed)
	return total, succed

def main():
	if len(sys.argv) < 2: return usage()

	input_name = sys.argv[1]
	output_name = sys.argv[2] if len(sys.argv) > 2 else input_name


	if os.path.isfile(input_name):
		do_encrypt(input_name, output_name)
		#do_decrypt(output_name, output_name + ".old")

	elif os.path.isdir(input_name):
		extensions = set([".glsl", ".fx"])
		encrypt_directory(input_name, extensions)

	else:
		print "invalid path: ", input_name

if __name__ == "__main__":
	main()
