
package.path = "scripts/?.lua;scripts/lib/?.lua"

function init()
	print("hello freegame")
	print("_G")
	print(freeg)
	print(freeg.RootScene)

	local a = freeg.RootScene:create()
	print(tolua.type(a))
	
	return true
end
