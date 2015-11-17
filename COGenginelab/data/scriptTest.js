
function TestingBehavior(rotateAnimId){
	print("constructor");
 this.othersEnded = false;
 this.fpsCounter = 0;
 this.rotateAnimId = rotateAnimId;
 print("FPS COUNTER: " + this.fpsCounter);
}


TestingBehavior.prototype.OnMessage = function(action, behaviorId){

  if(!this.othersEnded){
    if(action == 26 && behaviorId == this.rotateAnimId){
	  print("MOJO JAVASCRIPT HURRAY");
	  othersEnded = true;
	  OnTestAnimFinished(this.fpsCounter);
	}
  }
}

TestingBehavior.prototype.Update = function(delta, absolute){  
  this.fpsCounter++;
}