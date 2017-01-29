local GameObject  = {
    Input = nil
}

GameObject.__index = GameObject

function GameObject.new() 
    local self = setmetatable({}, GameObject)
    self.Input = Input.new()
    return self
end

function GameObject.getComponent(entity, name)
if name == "Transform" then return entity.gameObject:getTransform()
elseif name =="Material" then return entity.gameObject:getMaterial()
elseif name =="Body" then return entity.gameObject:getBody() 
else print("Error") return nil
end

end

return GameObject;