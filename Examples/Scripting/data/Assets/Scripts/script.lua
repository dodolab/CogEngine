 ScriptExample = Behavior:Extend("ScriptExample")

function ScriptExample:Constructor()
	self.imgIndex = 0
end


function ScriptExample:OnInit()
	self:SubscribeForMessages(StrId("ACT_BUTTON_CLICKED"))
end

function ScriptExample:OnMessage(msg)
	if (msg:HasAction(StrId("ACT_BUTTON_CLICKED")))
	 then
	   self.imgIndex = (self.imgIndex+1)%5
	   if(self.imgIndex == 0) then self.owner:SetImageMesh("rect_0.png") end
	   if(self.imgIndex == 1) then self.owner:SetImageMesh("rect_1.png") end
	   if(self.imgIndex == 2) then self.owner:SetImageMesh("rect_2.png") end
	   if(self.imgIndex == 3) then self.owner:SetImageMesh("rect_3.png") end
	   if(self.imgIndex == 4) then self.owner:SetImageMesh("rect_4.png") end
	 end
end

