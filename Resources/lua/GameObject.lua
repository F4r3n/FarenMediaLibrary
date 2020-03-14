GameObject = {}

function GameObject:create()
   local acnt = {}             
    setmetatable(acnt,self)
    print("Create")
   self.__index = self
   return acnt
end


function GameObject:GetTransform()
 return self._internal:GetTransform()
end


function GameObject:GetName()
 return self._internal:GetName()
end