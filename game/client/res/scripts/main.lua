
package.path = "scripts/?.lua;scripts/lib/?.lua"

function init()
	print("hello freegame")
	print("_G")
	print(freeg)
	print(freeg.RootScene)

	local root = freeg.RootScene:create()
	print(tolua.type(root))

	freeg.World:instance():setRoot(root)

	return true
end
