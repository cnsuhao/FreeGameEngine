
local __builtin = _G
local __modules = {}
local _loadfile = loadfile
local _pcall = pcall

function get_modules() return __modules end
function get_builtin() return __builtin end

function import(name)
	assert(type(name) == "string")
	local v = __modules[name]
	if v ~= nil then return v end

	return reload(name)
end

function reload(name)
	assert(type(name) == "string")
	return load_module(name)
end

function get_module(name)
	return __modules[name]
end

--------------------------------------------------------------------------
---
--------------------------------------------------------------------------
function load_module(name)
	local chunk = _loadfile(IMPORT_PATH .. name .. IMPORT_EXTENSION)
	if chunk == nil then
		local msg = string.format("Traceback: ImportError, Failed import module '%s'", name)
		error(msg, 3)
		return nil
	end

	local namespace = create_module(name)

	setfenv(chunk, namespace)
	local result, code = _pcall(chunk)
	if not result then
		print(code)
		local msg = string.format("Traceback: ImportError, Failed to excute module '%s'", name)
		error(msg, 3)
		return nil
	end

	return namespace
end

assert(module__index and "this function is implement by engine.")

function create_module(name)
	--assert(__modules[name] == nil)

	local namespace = {
		__name = name,
	}

	setmetatable(namespace, {__index = module__index})

	__modules[name] = namespace
	return namespace
end

--------------------------------------------------------------------------
---
--------------------------------------------------------------------------
-- disable the default module function
require  = nil
module   = nil

--------------------------------------------------------------------------
---
--------------------------------------------------------------------------
