
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

function onTouchBegin(x, y, btn)
	print("onTouchBegin", x, y, btn)
end

function onTouchEnd(x, y, btn)
	print("onTouchEnd", x, y, btn)
end

function onTouchMove(x, y, btn, dx, dy)
end

function onMouseMove(x, y, btn, dx, dy)
end

function onMouseWheel(dx, dy)
	print("onMouseWheel", dx, dy)
end

function onKeyEvent(isDown, code)
	print("onKeyEvent", isDown, code)
end
