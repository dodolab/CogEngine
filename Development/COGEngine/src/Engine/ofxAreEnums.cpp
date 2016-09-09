#include "ofxAreEnums.h"

/*! Action events */
namespace Actions{
	int BEHAVIOR_ADDED = 0;
	int BEHAVIOR_REMOVED = 1;
	int OBJECT_CREATED = 2;
	int OBJECT_REMOVED = 3;
	int USER_INPUT = 4;
	int COLLISION_OCURRED = 5;
	int NODE_DESTROYED = 6;
	int ATTRIBUTE_CHANGED = 7;
	int OBJECT_HIT_STARTED = 8;
	int OBJECT_HIT_OVER = 9;
	int OBJECT_HIT_LOST = 10;
	int OBJECT_HIT_ENDED = 11;
	int HEALTH_CHANGED = 12;
	int PARA_GROUNDED = 13;
	int PARA_KILLED = 14;
	int COPTER_KILLED = 15;
	int PLAYER_LOOSE = 16;
	int APP_FINISHED = 17;
	int TWEEN_ENDED = 18;
	int FLIP_ENDED = 19;
	int DELAY_ACTION = 20;
	int SCENE_SWITCHED = 21;
	int MATCH_FOUND = 22;
	int MATCH_FAILED = 23;
	int PROCESS_ENDED = 24;
	int NOAH_STATE_CHANGED = 25;
	int BEHAVIOR_FINISHED = 26;
	int TAKEOUTANIM_ENDED = 27;

}


/*! Attribute keys */
namespace Attrs{
	int VELOCITY = 0;
	int ACTIONS = 1;
	int ALLOWED_ACTIONS = 2;
	int BOUNDS = 3;
	int SIZE = 4;
	int POLYGON = 5;
	int COLOR = 6;
	int FRAME = 7;
	int COLLISIONS = 8;
	int HEALTH = 9;
	int SCORE = 10;
	int IMGBOUNDS = 11;
	int TEXT = 12;

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
}

