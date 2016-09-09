#pragma once

#include "ofMain.h"

typedef float(*FadeFunction)(float);

/*! User input action enumerator */
enum class Act{
	NONE=0,		/*!< none */
	LEFT=1,		/*!< move left */
	RIGHT=2,	/*!< move right */
	UP=3,		/*!< move up */
	DOWN=4,		/*!< move down */
	FIRE=5,		/*!< fire/action */
	SWITCH=6	/*!< switch/action */
};

/*! Scope object for messaging */
enum class ScopeType{
	ROOT,				 /*!< root object scope */
	SCENE,				 /*!< scene object scope */
	OBJECT,				 /*!< source object scope */
	CHILDREN,			 /*!< children objects scope */
	DIRECT_NO_TRAVERSE   /*!< no BubblingType made, objects are obtained from map */
};

/*! Action events */
namespace Actions{
	extern int BEHAVIOR_ADDED;
	extern int BEHAVIOR_REMOVED;
	extern int OBJECT_CREATED;
	extern int OBJECT_REMOVED;
	extern int USER_INPUT;
	extern int COLLISION_OCURRED;
	extern int NODE_DESTROYED;
	extern int ATTRIBUTE_CHANGED;
	extern int OBJECT_HIT_STARTED;
	extern int OBJECT_HIT_OVER;
	extern int OBJECT_HIT_LOST;
	extern int OBJECT_HIT_ENDED;
	extern int HEALTH_CHANGED;
	extern int PARA_GROUNDED;
	extern int PARA_KILLED;
	extern int COPTER_KILLED;
	extern int PLAYER_LOOSE;
	extern int APP_FINISHED;
	extern int TWEEN_ENDED;
	extern int FLIP_ENDED;
	extern int DELAY_ACTION;
	extern int SCENE_SWITCHED;
	extern int MATCH_FOUND;
	extern int MATCH_FAILED;
	extern int PROCESS_ENDED;
	extern int NOAH_STATE_CHANGED;
	extern int BEHAVIOR_FINISHED;
	extern int TAKEOUTANIM_ENDED;

	extern const char* ToString(int act);
}


/*! Attribute keys */
namespace Attrs{
	extern int VELOCITY;
	extern int ACTIONS;
	extern int ALLOWED_ACTIONS;
	extern int IMGSOURCE;
	extern int BOUNDS;
	extern int SIZE;
	extern int POLYGON;
	extern int COLOR;
	extern int FRAME;
	extern int COLLISIONS;
	extern int HEALTH;
	extern int FONT;
	extern int TEXT;
	extern int SCORE;
	extern int IMGBOUNDS;

	extern const char* ToString(int act);
}

/*! States*/
namespace States{
	extern int HITTABLE;
	extern int TO_LEFT;
	extern int TO_RIGHT;
	extern int HIT;
	extern int COLLID_SOURCE;
	extern int COLLID_TARGET;
	extern int FLIPPED;

	extern const char* ToString(int act);
}

/*! Behavior state enumerator */
enum class BehState {
	ACTIVE_ALL,			/*!< active for all events */
	ACTIVE_MESSAGES,	/*!< active for message sending */
	ACTIVE_UPDATES,		/*!< active for update */
	DISABLED			/*!< disabled completely */
};	

/*! Element type enumerator */
enum class ElemType {
	VIEW,				/*!< view element - used in DRAW loop */
	MODEL				/*!< model element - used in UPDATE loop */
};

/*! Node type enumerator */
enum class ObjType{
	ROOT,				/*!< root object, usually the topmost parent */
	SCENE,				/*!< scene root object, topmost parent in the scene */
	OBJECT,				/*!< common node  */
	HUD,				/*!< human interface object */
	INFO				/*!< info object */
};

