Test = {
    position= nil,
    otherGo = nil
}
function Test:start()
    for i=1,10 do 
        self.otherGo = CreateGameObject()
        local pos = self.otherGo:GetTransform():getPosition()
        --pos.x = pos.x+i
    end
    
    --self.position=self.Go:GetTransform():getPosition()
    --print(self.position.y)
    --print(self._internal:GetTransform())
    --print(self.Go.GetTransform().getPosition().x)
    --print(self.Go.GetTransform().getPosition())
end

function Test:update(dt)
    --self.position.x=self.position.x+1*dt

    --print(self.position.x)

end