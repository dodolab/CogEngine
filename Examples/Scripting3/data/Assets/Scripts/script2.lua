
-- inline initializer
ScriptExample2 = Behavior:Extend("ScriptExample2", {
	Constructor = function(self) self.animate = true end,
	OnInit = function(self) self:SubscribeForMessages(StrId("RED_ROTATION_COMPLETED")) end,
	OnMessage = function(self, msg)
		if (msg:HasAction(StrId("RED_ROTATION_COMPLETED")))
		then
			self.animate = true
		end
	end,

	Update = function(self, delta, absolute)
		if (self.animate)
		then
			rot = self.owner:GetTransform().rotation
			self.owner:GetTransform().rotation = self.owner:GetTransform().rotation + 10
			if (rot > 360)
			then
				self.owner:GetTransform().rotation = 0
				self.animate = false
				self:SendMessage(StrId("GREEN_ROTATION_COMPLETED"))
			end
		end
	end
} )
