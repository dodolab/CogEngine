#pragma once
#include "StrId.h"

namespace Cog {

	// ==========================================================================
	// This section contains actions, attribute names and state names
	// that are used as StrId objects in the engine

	/** Action events */
	extern StrId ACT_BEHAVIOR_ADDED;
	extern StrId ACT_BEHAVIOR_REMOVED;
	extern StrId ACT_OBJECT_CREATED;
	extern StrId ACT_OBJECT_REMOVED;
	extern StrId ACT_USER_INPUT;
	extern StrId ACT_COLLISION_OCURRED;
	extern StrId ACT_NODE_DESTROYED;
	extern StrId ACT_ATTRIBUTE_CHANGED;
	extern StrId ACT_OBJECT_HIT_STARTED;
	extern StrId ACT_OBJECT_HIT_OVER;
	extern StrId ACT_OBJECT_HIT_LOST;
	extern StrId ACT_OBJECT_HIT_ENDED;
	extern StrId ACT_APP_FINISHED;
	extern StrId ACT_TWEEN_ENDED;
	extern StrId ACT_DELAY_ACTION;
	extern StrId ACT_SCENE_SWITCHED;
	extern StrId ACT_PROCESS_ENDED;
	extern StrId ACT_BEHAVIOR_FINISHED;
	extern StrId ACT_STATE_CHANGED;
	extern StrId ACT_TRANSFORM_ENDED;
	extern StrId ACT_KEY_PRESSED;
	extern StrId ACT_MOUSE_SCROLLED;
	extern StrId ACT_SCREEN_CHANGED;
	extern StrId ACT_NET_MESSAGE_RECEIVED;
	extern StrId ACT_NET_CONNECTED;
	extern StrId ACT_NET_DISCONNECTED;
	extern StrId ACT_NET_CONNECTION_LOST;
	extern StrId ACT_NET_CALLBACK_RECEIVED;
	extern StrId ACT_UPDATE_MESSAGE_RECEIVED;
	extern StrId ACT_OBJECT_SELECTED;
	extern StrId ACT_BUTTON_CLICKED;
	extern StrId ACT_ATTR_CHANGED;

	/** Attribute keys */
	extern StrId ATTR_MOVEMENT;
	extern StrId ATTR_STEERING_BEH_DEST;
	extern StrId ATTR_STEERING_BEH_WANDER;
	extern StrId ATTR_ACTIONS;
	extern StrId ATTR_ALLOWED_ACTIONS;
	extern StrId ATTR_BOUNDS;
	extern StrId ATTR_SIZE;
	extern StrId ATTR_POLYGON;
	extern StrId ATTR_FRAME;
	extern StrId ATTR_COLLISIONS;
	extern StrId ATTR_IMGBOUNDS;
	extern StrId ATTR_SLIDER_VALUE;

	/** States*/
	extern StrId STATE_HITTABLE;
	extern StrId STATE_TOLEFT;
	extern StrId STATE_TORIGHT;
	extern StrId STATE_HIT;
	extern StrId STATE_COLLID_SOURCE;
	extern StrId STATE_COLLID_TARGET;
	extern StrId STATE_DISABLED;
	extern StrId STATE_ENABLED;
	extern StrId STATE_LOCKED;
	extern StrId STATE_SELECTED;

	/** Net messages */
	extern StrId NET_MSG_UPDATE;


	/** Paths */
#define PATH_MAIN_MANIFEST "COGAssets/Manifest.xml"
#define PATH_STRINGS "Assets/strings.xml"
#define PATH_DIMENSIONS "Assets/dimens.xml"
#define PATH_INTEGERS "Assets/integers.xml"
#define PATH_DEFAULT_SETTINGS "Assets/default_settings.xml"
#define PATH_GLOBAL_SETTINGS "Assets/global_settings.xml"
#define PATH_PROJECT_SETTINGS "Assets/project_settings.xml"
#define PATH_SCRIPTS "Assets/Scripts"
#define PATH_BASE_SCRIPTS "COGAssets/Scripts"
#define PATH_SPRITESHEETS "Assets/spritesheets.xml"
#define PATH_SCENES "Assets/scenes.xml"
#define PATH_NODES "Assets/nodes.xml"
#define PATH_ANIMATIONS "Assets/animations.xml"
#define PATH_ATTRANIMATIONS "Assets/attranimations.xml"
#define PATH_TRANSFORMS "Assets/transforms.xml"
#define PATH_BEHAVIORS "Assets/behaviors.xml"


// ============================ ENUMERATORS ======================================
// Meshes
#define MESH_NONE 1				/** Without mesh */
#define MESH_IMAGE 2			/** 2D image */
#define MESH_RECTANGLE 3		/** rectangle */
#define MESH_TEXT 4				/** 2D text */
#define MESH_LABEL 5			/** Text not affected by transformations */
#define MESH_SPRITE 6			/** 2D sprite */
#define MESH_MULTISPRITE 7		/** Collection of sprites */
#define MESH_BOUNDING_BOX 8		/** Box sizeable due to children nodes */
#define MESH_CIRCLE 9			/** Circle */

// Node running mode
#define RUNMODE_RUNNING 1			/** active for update and draw loops */
#define RUNMODE_PAUSED_ALL 2		/** active for draw loop */
#define RUNMODE_PAUSED_CHILDREN 3	/** active itself but children are inactive */
#define RUNMODE_PAUSED_ITSELF 4		/** inactive but children are active */
#define RUNMODE_INVISIBLE 5			/** active for update, inactive for draw */
#define RUNMODE_DISABLED 6			/** inactive for both update and draw */

// Node type
#define NODETYPE_ROOT 1				/** root object, usually the topmost parent */
#define NODETYPE_SCENE 2			/** scene root object, the root object of each scene */
#define NODETYPE_OBJECT 3			/** general node  */

// Type of the transformation calculation
#define CALCTYPE_ABS 1		/** absolute (un) */
#define CALCTYPE_LOC 2		/** local (-) */
#define CALCTYPE_PER 3		/** percentage <0..1> (r) */
#define CALCTYPE_GRID 4		/** grid positioning (gr) */
#define CALCTYPE_ABS_PER 5  /** absolute percentage (rp) */

//Tweening direction
#define TWEEN_NONE 1	/** direct tween */
#define TWEEN_LEFT 2	/** move to the left*/
#define TWEEN_RIGHT 3	/** move to the right */
#define TWEEN_UP 4		/** move up*/
#define TWEEN_DOWN 5	/** move down */

}// namespace