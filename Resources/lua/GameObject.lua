GameObject = {}

function GameObject:create()
   local acnt = {}             -- our new object
    setmetatable(acnt,self)  -- make Account handle lookup

   self.__index = self
   return acnt
end

function GameObject:print()
	print(self.balance)
end

function GameObject:Test()
  print("yolo")
  print(self._internal:GetTransform())
end

