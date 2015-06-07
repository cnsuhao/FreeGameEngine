# -*- coding: utf-8 -*-

import json
import interface

def parse_method_name(name):
	if not name.startswith("web"):
		print "warn: the name ", name, " doesn't start with 'web'"
		return None

	name = name[ 3: ]
	pos = name.find('_')
	if pos < 0:
		print "warn: the name ", name, "doesn't contain '_'"
		return None

	handle = name[ :pos ]
	part = name[ pos+1: ]

	return (handle.lower(), part.lower())

def parse_method(handle, part, value):

	format = "handle=%s&part=%s" % (handle, part)
	keys = ""
	values = ""
	checks = "\tassert(type(self) == 'table')\n"

	for i, (arg, typ) in enumerate(value):
		
		if i != 0:
			keys += ", "
			values += ", "
		keys += arg

		format += "&%s=" % arg
		if typ == int:
			format += "%d"
			values += "math.floor(%s)" % (arg, )
			checks += "\tassert(type(%s) == 'number')\n" % (arg, )

		elif typ == float:
			format += "%f"
			values += arg
			checks += "\tassert(type(%s) == 'number')\n" % (arg, )

		elif typ == str:
			format += "%s"
			values += arg
			checks += "\tassert(type(%s) == 'string')\n" % (arg, )

		elif typ == json.loads:
			format += "%s"
			values += "libjson.encode(%s)" % (arg, )
			checks += "\tassert(type(%s) == 'table')\n" % (arg, )

		else:
			print "warn: unsported argument type", typ, "when parse '", handle, part, "' arg ", arg
			return None

	ret  = "function TBase:%s_%s(%s)\n" % (part, handle, keys, )
	ret += checks + "\n"
	ret += "\tlocal data = string.format('%s',\n\t\t%s)\n" % (format, values)
	ret += "\tself:http_get(data, {%s}, 'on_%s_%s')\n" % (keys, part, handle)
	ret += "end\n\n"

	return ret

def converter(infos):
	ret  = "-- this file is auto generated, don't try to modify.\n\n"
	ret += "require 'client/avatar_net'\n"
	ret += "setfenv(1, avatar_net)\n\n"

	keys = []
	for name in infos:
		method = parse_method_name(name)
		if method is None: continue

		handle, part = method
		keys.append( (handle, part, name) )

	keys.sort(lambda x, y: cmp(x[1], y[1]))
	for handle, part, name in keys:
		tmp = parse_method(handle, part, infos[name])
		if tmp is not None: ret += tmp

	return ret

def main():
	ret = converter(interface.METHOD_ARG_MAP)
	if len(ret) == 0:
		print "error: the result len is zero."
		return

	hfile = open("../../res/scripts/client/avatar_interface.lua", "w")
	hfile.write(ret)
	hfile.close()

if __name__ == "__main__":
	main()



