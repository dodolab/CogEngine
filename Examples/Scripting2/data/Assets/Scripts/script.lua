  ScriptExample = Behavior:Extend("ScriptExample")

function ScriptExample:OnInit()
	self:SubscribeForMessages(StrId("ACT_BUTTON_CLICKED"))
end

function ScriptExample:Update(delta, absolute)
	self.owner.transform.rotation = self.owner.transform.rotation+1
end

