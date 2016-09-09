/*! Types of traversation */
namespace Traverses{
	int CHILD_FIRST = 0;	/*!< child is updated first */
	int BEH_FIRST = 1;		/*!< behaviors are updated first */
	int ROOT = 2;			/*!< root is updated first */
	int SCENEROOT = 3;		/*!< scene root is updated first */
}

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
	int OBJECT_HIT = 8;
	int OBJECT_RELEASED = 9;
	int HEALTH_CHANGED = 10;
	int PARA_GROUNDED = 11;
	int PARA_KILLED = 12;
	int COPTER_KILLED = 13;
	int PLAYER_LOOSE = 14;
	int GAME_FINISHED = 15;
}


/*! Attribute keys */
namespace Attrs{
	int VELOCITY = 1;
	int ACTIONS = 2;
	int ALLOWED_ACTIONS = 3;
	int IMGSOURCE = 4;
	int BOUNDS = 5;
	int SIZE = 6;
	int POLYGON = 7;
	int COLOR = 8;
	int FRAME = 9;
	int COLLISIONS = 10;
	int HEALTH = 11;
	int FONT = 12;
	int TEXT = 13;
	int SCORE = 14;
}

/*! States*/
namespace States{
	 int HITTABLE = 1;
	 int TO_LEFT=2;
	 int TO_RIGHT=3;
	 int HIT = 4;
	 int COLLID_SOURCE = 5;
	 int COLLID_TARGET = 6;
}