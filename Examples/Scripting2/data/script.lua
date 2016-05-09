 ScriptExample = { 
   imgIndex = 0,
   OnInit = function() 
   end, 
   OnMessage = function(msg) 
   end, 
   Update = function(delta, absolute)
     ScriptExample_beh.owner.transform.rotation = ScriptExample_beh.owner.transform.rotation+1
   end 
} 

ScriptExample_beh = Behavior() 
ScriptExample_beh:Register(ScriptExample, "ScriptExample")