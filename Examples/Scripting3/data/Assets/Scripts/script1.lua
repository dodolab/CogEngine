ScriptExample = Behavior:Extend("ScriptExample")

function ScriptExample:Constructor()
	self.animate = false
end


function ScriptExample:OnInit()
	self:SubscribeForMessages(StrId("GREEN_ROTATION_COMPLETED"))
end

function ScriptExample:OnMessage(msg)
	if (msg:HasAction(StrId("GREEN_ROTATION_COMPLETED")))
	then
		self.animate = true
	end
end

function ScriptExample:Update(delta, absolute)
	if (self.animate)
	then
		rot = self.owner:GetTransform().rotation
		self.owner:GetTransform().rotation = self.owner:GetTransform().rotation + 10
		if (rot > 360)
		then
			self.owner:GetTransform().rotation = 0
			self.animate = false
			self:SendMessage(StrId("RED_ROTATION_COMPLETED"))
		end
	end
end


test = "{\"json test\" : 12345}"

jsonTxt = JSON:decode(test)
print(jsonTxt.mojo)