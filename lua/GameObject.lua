local GameObject  = {
    gameObject = nil
}

GameObject.__index = GameObject

function GameObject.new() 
    local self = setmetatable({}, GameObject)
    return self
end

function getValueScript(nameScript, nameVariable)
    assert(nameScript ~= "")
    assert(nameVariable ~= "")
    return load("return "..nameScript.."."..nameVariable)()
end

return GameObject;