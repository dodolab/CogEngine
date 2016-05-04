 ScriptExample = { 
   animate = false,
   OnInit = function() 
     ScriptExample_beh:SubscribeForMessages(StrId("GREEN_ROTATION_COMPLETED"))
   end, 
   OnMessage = function(msg) 
     if(msg:HasAction(StrId("GREEN_ROTATION_COMPLETED")))
	 then
	   ScriptExample.animate = true;
	 end
   end, 
   Update = function(delta, absolute)
     if(ScriptExample.animate)
	 then
       rot = ScriptExample_beh.owner.transform.rotation
       ScriptExample_beh.owner.transform.rotation = ScriptExample_beh.owner.transform.rotation+10
	   if(rot > 360)
	   then
	     ScriptExample_beh.owner.transform.rotation = 0
		 ScriptExample.animate = false
		 ScriptExample_beh:SendMessage(StrId("RED_ROTATION_COMPLETED"))
	   end
     end
   end 
} 

ScriptExample_beh = Behavior() 
ScriptExample_beh:Register(ScriptExample, "ScriptExample")