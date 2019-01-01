Script = {}

function Script:create(balance)
   local acnt = {}             -- our new object
    setmetatable(acnt,self)  -- make Account handle lookup

   self.__index = self
   acnt.balance = balance      -- initialize our object
   --acnt.__add = Account.add
   print("create Script")
   return acnt
end

function Script:withdraw(amount)
   self.balance = self.balance - amount
end

function Script:print()
	print(self.balance)
end

function Script.__add(a, b)
	local acc = Script:create(a.balance)
	acc.balance = acc.balance + b.balance
	return acc
end