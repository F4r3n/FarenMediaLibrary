local GameObject  = {
    gameObject = nil,
    Input = nil
}

GameObject.__index = GameObject

function GameObject.new() 
    local self = setmetatable({}, GameObject)
    self.Input = Input.new()
    return self
end


return GameObject;