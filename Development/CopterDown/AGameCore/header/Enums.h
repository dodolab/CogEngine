
#ifndef ENUMS_H
#define ENUMS_H

enum class Act{LEFT, RIGHT, UP, DOWN, FIRE, SWITCH};

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

namespace Attrs{
	int VELOCITY = 17;
	int USERACTION = 16;
	int IMGSOURCE = 24;
	int BOUNDS = 26;
}

enum class BehState {ACTIVE_ALL, ACTIVE_MESSAGES, ACTIVE_UPDATES, DISABLED};

enum class ElemType {ALL, VIEW, MODEL};

enum class ObjType{ROOT, SCENE, OBJECT, HUD, INFO};

enum class SenderType{GAMEOBJ, BEHAVIOR, ATTR};

namespace Traverses{
	int CHILD_FIRST = 0;
	int BEH_FIRST = 1;
	int ROOT = 2;
	int SCENEROOT = 3;
}

#endif
