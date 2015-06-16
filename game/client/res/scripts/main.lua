
package.path = "scripts/?.lua;scripts/lib/?.lua"

IMPORT_PATH = "scripts/"
IMPORT_EXTENSION = ".lua"

require "module"

local keys = import("keycode").keycode

function isRetail() return false end

function Vector2(x, y) return {x = x, y = y} end
function Vector3(x, y, z) return {x = x, y = y, z = z } end
function Vector4(x, y, z, w) return {x = x, y = y, z = z, w = w } end
function Quaternion(x, y, z, w) return {x = x, y = y, z = z, w = w } end


function init()
	print("hello freegame")
	print("_G", _G, #_G)
	print("frg", frg, #frg)
	print("frg.RootScene", frg.RootScene)

	local root = frg.RootScene:create()
	print(tolua.type(root))

	frg.World:instance():setRoot(root)

	local camera = frg.World:instance():getCamera()
	camera:setControl(true)

	local mesh = frg.MeshNode:create("model/homeland/90_100_10.mesh")
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
	handleMouseCamera(x, y, btn, dx, dy)
end

function onMouseMove(x, y, btn, dx, dy)
end

function onMouseWheel(dx, dy)
	print("onMouseWheel", dx, dy)
end

function onKeyEvent(down, key)
	print("onKeyEvent", down, key)

	handleKeyCamera(down, key)
end


function handleKeyCamera(down, key)
	if isRetail() then return end

	local camera = frg.World:instance():getCamera()
	if camera == nil then return end

	if down then
		local speed = 5.0
		if key == keys.KEY_A then
			camera:doTranslation(camera:getRightVector(), -speed)
		elseif key == keys.KEY_D then
			camera:doTranslation(camera:getRightVector(), speed)
		end

		if key == keys.KEY_W then
			camera:doTranslation(camera:getForwardVector(), speed)
		elseif key == keys.KEY_S then
			camera:doTranslation(camera:getForwardVector(), -speed)
		end

		local speed = math.pi / 4.0
		if key == keys.KEY_UP then
			camera:doRotation(camera:getRightVector(), -speed)
		elseif key == keys.KEY_DOWN then
			camera:doRotation(camera:getRightVector(), speed)
		end

		if key == keys.KEY_LEFT then
			camera:doRotation(Vector3(0, 1, 0), speed);
		elseif key == keys.KEY_RIGHT then
			camera:doRotation(Vector3(0, 1, 0), -speed)
		end
	else
		if key == keys.KEY_A or
			key == keys.KEY_D or
			key == keys.KEY_W or
			key == keys.KEY_S then
			camera:doTranslation(Vector3(0, 0, 0), 0.0)
		end

		if key == keys.KEY_UP or
			key == keys.KEY_DOWN or
			key == keys.KEY_LEFT or
			key == keys.KEY_RIGHT then
			camera:doRotation(Vector3(0, 0, 0), 0.0)
		end
	end
end

function handleMouseCamera(x, y, btn, dx, dy)
	if isRetail() then return end

	local camera = frg.World:instance():getCamera()
	if camera == nil then return end

	local size = frg.World:instance():getViewSize()

	if math.abs(dx) > 0.0001 then
		local angle = dx / size.x * math.pi
		local center = camera:getPosition()

		camera:rotateAlong(center, Vector3(0, 1, 0), angle)
	end

	if math.abs(dy) > 0.0001 then
		local angle = dy / size.y * math.pi
		local center = camera:getPosition()
		local axis = camera:getRightVector()

		camera:rotateAlong(center, axis, angle)
	end	
end
