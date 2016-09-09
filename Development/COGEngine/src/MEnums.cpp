#include <string>
#include <stdarg.h>  // For va_start, etc.
#include <cmath>
#include <memory>


/*! Action events */
namespace Actions{
	int BEHAVIOR_ADDED = 0;
	int BEHAVIOR_REMOVED = 1;
	int OBJECT_CREATED = 2;
	int OBJECT_REMOVED = 3;
	int USER_INPUT = 4;
	int COLLISION_OCURRED = 5;
	int GAMEOBJECT_DESTROYED = 6;
	int ATTRIBUTE_CHANGED = 7;
	int OBJECT_HIT_STARTED = 8;
	int OBJECT_HIT_OVER = 9;
	int OBJECT_HIT_LOST = 10;
	int OBJECT_HIT_ENDED = 11;
	int HEALTH_CHANGED = 13;
	int PARA_GROUNDED = 14;
	int PARA_KILLED = 15;
	int COPTER_KILLED = 16;
	int PLAYER_LOOSE = 17;
	int GAME_FINISHED = 18;
	int TWEEN_ENDED = 19;
	int FLIP_ENDED = 20;
	int DELAY_ACTION = 21;
	int SCENE_SWITCHED = 22;
	int MATCH_FOUND = 23;
	int MATCH_FAILED = 24;
	int PROCESS_ENDED = 25;
	int NOAH_STATE_CHANGED = 26;
	int BEHAVIOR_FINISHED = 27;
	int TAKEOUTANIM_ENDED = 28;

	const char* ToString(int act){
		const char * vals[] = { 
			"BEHAVIOR_ADDED", 
			"BEHAVIOR_REMOVED", 
			"OBJECT_CREATED",
			"OBJECT_REMOVED",
			"USER_INPUT",
			"COLLISION_OCURRED",
			"GAMEOBJECT_DESTROYED",
			"ATTRIBUTE_CHANGED",
			"OBJECT_HIT_STARTED",
			"OBJECT_HIT_OVER",
			"OBJECT_HIT_LOST",
			"OBJECT_HIT_ENDED",
			"HEALTH_CHANGED",
			"PARA_GROUNDED",
			"PARA_KILLED",
			"COPTER_KILLED",
			"PLAYER_LOOSE",
			"GAME_FINISHED",
			"TWEEN_ENDED",
			"SLIDE_ENDED",
			"DELAY_ACTION",
			"SCENE_SWITCHED",
			"MATCH_FOUND",
			"MATCH_FAILED",
			"PROCESS_ENDED",
			"NOAH_STATE_CHANGED",
			"BEHAVIOR_FINISHED",
			"TAKEOUTANIM_ENDED"
		};
		return vals[act];
	}
}


/*! Attribute keys */
namespace Attrs{
	int VELOCITY = 0;
	int ACTIONS = 1;
	int ALLOWED_ACTIONS = 2;
	int IMGSOURCE = 3;
	int BOUNDS = 4;
	int SIZE = 5;
	int POLYGON = 6;
	int COLOR = 7;
	int FRAME = 8;
	int COLLISIONS = 9;
	int HEALTH = 10;
	int FONT = 11;
	int TEXT = 12;
	int SCORE = 13;

	const char* ToString(int act){
		const char * vals[] = {
			"VELOCITY",
			"ACTIONS",
			"ALLOWED_ACTIONS",
			"IMGSOURCE",
			"BOUNDS",
			"SIZE",
			"POLYGON",
			"COLOR",
			"FRAME",
			"COLLISIONS",
			"HEALTH",
			"FONT",
			"TEXT",
			"SCORE"
		};
		return vals[act];
	}
}

/*! States*/
namespace States{
	 int HITTABLE = 1;
	 int TO_LEFT=2;
	 int TO_RIGHT=3;
	 int HIT = 4;
	 int COLLID_SOURCE = 5;
	 int COLLID_TARGET = 6;
	 int FLIPPED = 7;

	 const char* ToString(int act){
		 const char * vals[] = {
			 "HITTABLE",
			 "TO_LEFT",
			 "TO_RIGHT",
			 "HIT",
			 "COLLID_SOURCE",
			 "COLLID_TARGET",
			 "FLIPPED"
		 };
		 return vals[act];
	 }
}

