

SceneSwitcher = Behavior:Extend("SceneSwitcher", {
	OnInit = function(self) self:SubscribeForMessages(StrId("BUTTON_CLICKED")) end,
	OnMessage = function(self, msg)
		if (msg:HasAction(StrId("BUTTON_CLICKED")))
		then
			stage = CogGetComponent("Stage")	
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
} )
