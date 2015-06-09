
package.path = "scripts/?.lua;scripts/lib/?.lua"

function init()
	print("hello freegame")
	print("_G", _G, #_G)
	print("freeg", freeg, #freeg)
	print("freeg.RootScene", freeg.RootScene)

	local root = freeg.RootScene:create()
	print(tolua.type(root))

	freeg.World:instance():setRoot(root)

	return true
end
