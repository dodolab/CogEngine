 ScriptExample2 = { 
   animate = true,
   OnInit = function() 
     ScriptExample2_beh:SubscribeForMessages(StrId("RED_ROTATION_COMPLETED"))
   end, 
   OnMessage = function(msg) 
     if(msg:HasAction(StrId("RED_ROTATION_COMPLETED")))
	 then
	   ScriptExample2.animate = true;
	 end
   end, 
   Update = function(delta, absolute)
     if(ScriptExample2.animate)
	 then
       rot = ScriptExample2_beh.owner.transform.rotation
       ScriptExample2_beh.owner.transform.rotation = ScriptExample2_beh.owner.transform.rotation+10
	   if(rot > 360)
	   then
	     ScriptExample2_beh.owner.transform.rotation = 0
		 ScriptExample2.animate = false
		 ScriptExample2_beh:SendMessage(StrId("GREEN_ROTATION_COMPLETED"))
	   end
     end
   end 
} 

ScriptExample2_beh = Behavior() 
ScriptExample2_beh:Register(ScriptExample2, "ScriptExample2")