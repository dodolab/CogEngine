 ScriptExample = { 
   imgIndex = 0,
   OnInit = function() 
     ScriptExample_beh:SubscribeForMessages(StrId("BUTTON_CLICKED")) 
   end, 
   OnMessage = function(msg) 
     if(msg:HasAction(StrId("BUTTON_CLICKED")))
	 then
	   ScriptExample.imgIndex = (ScriptExample.imgIndex+1)%5
	   if(ScriptExample.imgIndex == 0) then ScriptExample_beh.owner:SetImageMesh("rect_0.png") end
	   if(ScriptExample.imgIndex == 1) then ScriptExample_beh.owner:SetImageMesh("rect_1.png") end
	   if(ScriptExample.imgIndex == 2) then ScriptExample_beh.owner:SetImageMesh("rect_2.png") end
	   if(ScriptExample.imgIndex == 3) then ScriptExample_beh.owner:SetImageMesh("rect_3.png") end
	   if(ScriptExample.imgIndex == 4) then ScriptExample_beh.owner:SetImageMesh("rect_4.png") end
	 end
   end, 
   Update = function(delta, absolute) 
   end 
} 

ScriptExample_beh = Behavior() 
ScriptExample_beh:Register(ScriptExample, "ScriptExample")