GameObject = {}

function GameObject:create()
   local acnt = {}             
    setmetatable(acnt,self)
   self.__index = self
   return acnt
end


function GameObject:GetTransform()
 return self._internal:GetTransform()
end

function GameObject:GetBody3D()
 return self._internal:GetBody3D()
end


function GameObject:GetName()
 return self._internal:GetName()
end