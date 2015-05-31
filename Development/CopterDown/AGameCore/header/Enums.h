#ifndef ENUMS_H
#define ENUMS_H

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

/*! Types of traversation */
namespace Traverses{
	extern int CHILD_FIRST;	/*!< child is updated first */
	extern int BEH_FIRST;		/*!< behaviors are updated first */
	extern int ROOT;			/*!< root is updated first */
	extern int SCENEROOT;		/*!< scene root is updated first */
}

/*! Action events */
namespace Actions{
	extern int BEHAVIOR_ADDED;
	extern int BEHAVIOR_REMOVED;
	extern int OBJECT_CREATED;
	extern int OBJECT_REMOVED;
	extern int USER_INPUT;
	extern int COLLISION_OCURRED;
	extern int GAMEOBJECT_DESTROYED;
	extern int ATTRIBUTE_CHANGED;
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
	extern int HIT_IMAGE;
}

/*! States*/
namespace States{
	extern int HITTABLE;
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
	ALL,				/*!< common type */
	VIEW,				/*!< view element - is not a part of game model */
	MODEL				/*!< model element - is a part of game machine */
};

/*! Game object type enumerator */
enum class ObjType{
	ROOT,				/*!< root object, usually the topmost parent */
	SCENE,				/*!< scene root object, topmost parent in the scene */
	OBJECT,				/*!< simple game object */
	HUD,				/*!< human interface object */
	INFO				/*!< info object */
};

/*! Message sender type enumerator */
enum class SenderType{
	GAMEOBJ,			/*!< sender is game object */
	BEHAVIOR,			/*!< sender is behavior */
	ATTR				/*!< sender is attribute */
};

#endif
