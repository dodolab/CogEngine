  ScriptExample = Behavior:Extend("ScriptExample")

function ScriptExample:OnInit()
	self:SubscribeForMessages(StrId("ACT_BUTTON_CLICKED"))
end

function ScriptExample:Update(delta, absolute)
	self.owner:GetTransform().rotation = self.owner:GetTransform().rotation+1
end

