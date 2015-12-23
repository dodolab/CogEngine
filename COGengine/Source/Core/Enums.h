#pragma once

#define PIF 3.141592653f

namespace Cog {

	typedef float(*FadeFunction)(float);

	/*! User input action enumerator */
	enum class RenderType {
		NONE, IMAGE, RECTANGLE, POLYGON, ARC, TEXT, SPRITE
	};

	/*! User input action enumerator */
	enum class Act {
		NONE = 0,		/*!< none */
		LEFT = 1,		/*!< move left */
		RIGHT = 2,	/*!< move right */
		UP = 3,		/*!< move up */
		DOWN = 4,		/*!< move down */
		FIRE = 5,		/*!< fire/action */
		SWITCH = 6	/*!< switch/action */
	};

	/*! Scope object for messaging */
	enum class Scope {
		ROOT,				 /*!< root object scope */
		SCENE,				 /*!< scene object scope */
		OBJECT,				 /*!< source object scope */
		CHILDREN,			 /*!< children objects scope */
		DIRECT_NO_TRAVERSE   /*!< no BubblingType made, objects are obtained from map */
	};

	/*! Action events */
	#define ACT_BEHAVIOR_ADDED "BEHAVIOR_ADDED"
	#define ACT_BEHAVIOR_REMOVED "BEHAVIOR_REMOVED"
	#define ACT_OBJECT_CREATED "OBJECT_CREATED"
	#define ACT_OBJECT_REMOVED "OBJECT_REMOVED"
	#define ACT_USER_INPUT "USER_INPUT"
	#define ACT_COLLISION_OCURRED "COLLISION_OCURRED"
	#define ACT_NODE_DESTROYED "NODE_DESTROYED"
	#define ACT_ATTRIBUTE_CHANGED "ATTRIBUTE_CHANGED"
	#define ACT_OBJECT_HIT_STARTED "OBJECT_HIT_STARTED"
	#define ACT_OBJECT_HIT_OVER "OBJECT_HIT_OVER"
	#define ACT_OBJECT_HIT_LOST "OBJECT_HIT_LOST"
	#define ACT_OBJECT_HIT_ENDED "OBJECT_HIT_ENDED"
	#define ACT_APP_FINISHED "APP_FINISHED"
	#define ACT_TWEEN_ENDED "TWEEN_ENDED"
	#define ACT_FLIP_ENDED "FLIP_ENDED"
	#define ACT_DELAY_ACTION "DELAY_ACTION"
	#define ACT_SCENE_SWITCHED "SCENE_SWITCHED"
	#define ACT_PROCESS_ENDED "PROCESS_ENDED"
	#define ACT_BEHAVIOR_FINISHED "BEHAVIOR_FINISHED"
	#define ACT_STATE_CHANGED "STATE_CHANGED"
	#define ACT_TRANSFORM_ENDED  "TRANSFORM_ENDED"

	/*! Attribute keys */
	#define ATTR_VELOCITY "VELOCITY"
	#define ATTR_ACTIONS "ACTIONS"
	#define ATTR_ALLOWED_ACTIONS "ALLOWED_ACTIONS"
	#define ATTR_BOUNDS "BOUNDS"
	#define ATTR_SIZE "SIZE"
	#define ATTR_POLYGON "POLYGON"
	#define ATTR_FRAME "FRAME"
	#define ATTR_COLLISIONS "COLLISIONS"
	#define ATTR_IMGBOUNDS "IMGBOUNDS"

	/*! States*/
	#define STATES_HITTABLE "HITTABLE"
	#define STATES_TOLEFT "TO_LEFT"
	#define STATES_TORIGHT "TO_RIGHT"
	#define STATES_HIT "HIT"
	#define STATES_COLLID_SOURCE "COLLID_SOURCE"
	#define STATES_COLLID_TARGET "COLLID_TARGET"
    #define STATES_DISABLED "DISABLED"
	#define STATES_ENABLED "ENABLED"
	#define STATES_LOCKED "LOCKED"

	/*! Listener state enumerator */
	enum class ListenerState {
		ACTIVE_ALL,			/*!< active for all events */
		ACTIVE_MESSAGES,	/*!< active for message sending */
		ACTIVE_UPDATES,		/*!< active for update */
		DISABLED			/*!< disabled completely */
	};


	/*! Node type enumerator */
	enum class ObjType {
		ROOT,				/*!< root object, usually the topmost parent */
		SCENE,				/*!< scene root object, topmost parent in the scene */
		OBJECT,				/*!< common node  */
		HUD,				/*!< human interface object */
		INFO				/*!< info object */
	};

}// namespace