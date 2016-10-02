ScriptExample2 = Behavior:Extend("ScriptExample2")
ScriptExample2.animate = true

function ScriptExample2:OnInit()
  self:SubscribeForMessages(StrId("RED_ROTATION_COMPLETED"))
end

function ScriptExample2:OnMessage(msg)
    if(msg:HasAction(StrId("RED_ROTATION_COMPLETED")))
	  then
	   self.animate = true
	end
end

function ScriptExample2:Update(delta, absolute)
    if(self.animate)
	then
        rot = self.owner.transform.rotation
        self.owner.transform.rotation = self.owner.transform.rotation+10
	    if(rot > 360)
	    then
	      self.owner.transform.rotation = 0
		  self.animate = false
		  self:SendMessage(StrId("GREEN_ROTATION_COMPLETED"))
	   end
     end
end


-- create instance (not needed anymore)
--scriptExampleInst2 = ScriptExample2:New()

