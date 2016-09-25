local GameObject  = {
    gameObject = nil
}

GameObject.__index = GameObject

function GameObject.new() 
    local self = setmetatable({}, GameObject)
    return self
end


return GameObject;