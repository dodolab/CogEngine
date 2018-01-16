SceneSwitcher = Behavior:Extend("SceneSwitcher")

function SceneSwitcher:Constructor()
	self.animate = false
end


function SceneSwitcher:OnInit()
	self:SubscribeForMessages(StrId("ACT_BUTTON_CLICKED"))
end

function SceneSwitcher:OnMessage(msg)
	if (msg:HasAction(StrId("ACT_BUTTON_CLICKED")))
	then
			stage = CogGetComponent_Stage()	
		    actualScene = stage.actualScene.name
			newScene = (actualScene == "scene1" and  "scene2" or "scene1")
			
			if(msg.contextNode.tag == "previous_but") then
			  stage:SwitchToScene(stage:FindSceneByName(newScene), TweenDirection_RIGHT)
			end
			
			if(msg.contextNode.tag == "next_but") then
			  stage:SwitchToScene(stage:FindSceneByName(newScene), TweenDirection_LEFT)
			end
	end
end
