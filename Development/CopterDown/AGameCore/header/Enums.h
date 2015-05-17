
#ifndef ENUMS_H
#define ENUMS_H

/*! User input action enumerator */
enum class Act{
	LEFT,		/*!< move left */
	RIGHT,		/*!< move right */
	UP,			/*!< move up */
	DOWN,		/*!< move down */
	FIRE,		/*!< fire/action */
	SWITCH		/*!< switch/action */
};

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
}

/*! Attribute keys */
namespace Attrs{
	int VELOCITY = 17;
	int USERACTION = 16;
	int IMGSOURCE = 24;
	int BOUNDS = 26;
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

/*! Types of traversation */
namespace Traverses{
	int CHILD_FIRST = 0;	/*!< child is updated first */
	int BEH_FIRST = 1;		/*!< behaviors are updated first */
	int ROOT = 2;			/*!< root is updated first */
	int SCENEROOT = 3;		/*!< scene root is updated first */
}

#endif
