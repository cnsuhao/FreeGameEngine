
package.path = "scripts/?.lua;scripts/lib/?.lua"

function init()
	print("hello freegame")
	print("_G", _G, #_G)
	print("freeg", freeg, #freeg)
	print("freeg.RootScene", freeg.RootScene)

	local root = freeg.RootScene:create()
	print(tolua.type(root))

	freeg.World:instance():setRoot(root)

	local mesh = freeg.MeshNode:create("model/homeland/90_100_10.mesh")
	if mesh then
		root:addChild(mesh)
	end

	return true
end
