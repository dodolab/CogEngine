#pragma once


namespace Cog {

// ==========================================================================
// This section contains actions, attribute names and state names
// that are used as StrId objects in the engine

/** Action events */
#define ACT_BEHAVIOR_ADDED "ACT_BEHAVIOR_ADDED"
#define ACT_BEHAVIOR_REMOVED "ACT_BEHAVIOR_REMOVED"
#define ACT_OBJECT_CREATED "ACT_OBJECT_CREATED"
#define ACT_OBJECT_REMOVED "ACT_OBJECT_REMOVED"
#define ACT_USER_INPUT "ACT_USER_INPUT"
#define ACT_COLLISION_OCURRED "ACT_COLLISION_OCURRED"
#define ACT_NODE_DESTROYED "ACT_NODE_DESTROYED"
#define ACT_ATTRIBUTE_CHANGED "ACT_ATTRIBUTE_CHANGED"
#define ACT_OBJECT_HIT_STARTED "ACT_OBJECT_HIT_STARTED"
#define ACT_OBJECT_HIT_OVER "ACT_OBJECT_HIT_OVER"
#define ACT_OBJECT_HIT_LOST "ACT_OBJECT_HIT_LOST"
#define ACT_OBJECT_HIT_ENDED "ACT?OBJECT_HIT_ENDED"
#define ACT_APP_FINISHED "ACT_APP_FINISHED"
#define ACT_TWEEN_ENDED "ACT_TWEEN_ENDED"
#define ACT_DELAY_ACTION "ACT_DELAY_ACTION"
#define ACT_SCENE_SWITCHED "ACT_SCENE_SWITCHED"
#define ACT_PROCESS_ENDED "ACT_PROCESS_ENDED"
#define ACT_BEHAVIOR_FINISHED "ACT_BEHAVIOR_FINISHED"
#define ACT_STATE_CHANGED "ACT_STATE_CHANGED"
#define ACT_TRANSFORM_ENDED  "ACT_TRANSFORM_ENDED"
#define ACT_KEY_PRESSED "ACT_KEY_PRESSED"
#define ACT_MOUSE_SCROLLED "ACT_MOUSE_SCROLLED"
#define ACT_SCREEN_CHANGED "ACT_SCREEN_CHANGED"
#define ACT_NET_MESSAGE_RECEIVED "ACT_NET_MESSAGE_RECEIVED"
#define ACT_NET_CONNECTED "ACT_NET_CONNECTED"
#define ACT_NET_DISCONNECTED "ACT_NET_DISCONNECTED"
#define ACT_NET_CONNECTION_LOST "ACT_NET_CONNECTION_LOST"
#define ACT_NET_CALLBACK_RECEIVED "ACT_NET_CALLBACK_RECEIVED"
#define ACT_UPDATE_MESSAGE_RECEIVED "ACT_UPDATE_MESSAGE_RECEIVED"
#define ACT_OBJECT_SELECTED "ACT_OBJECT_SELECTED"
#define ACT_BUTTON_CLICKED "ACT_BUTTON_CLICKED"
#define ACT_ATTR_CHANGED "ACT_ATTR_CHANGED"

/** Attribute keys */
#define ATTR_MOVEMENT "ATTR_MOVEMENT"
#define ATTR_STEERING_BEH_DEST "ATTR_STEERING_BEH_DEST"
#define ATTR_STEERING_BEH_WANDER "ATTR_STEERING_BEH_WANDER"
#define ATTR_ACTIONS "ATTR_ACTIONS"
#define ATTR_ALLOWED_ACTIONS "ATTR_ALLOWED_ACTIONS"
#define ATTR_BOUNDS "ATTR_BOUNDS"
#define ATTR_SIZE "ATTR_SIZE"
#define ATTR_POLYGON "ATTR_POLYGON"
#define ATTR_FRAME "ATTR_FRAME"
#define ATTR_COLLISIONS "ATTR_COLLISIONS"
#define ATTR_IMGBOUNDS "ATTR_IMGBOUNDS"
#define ATTR_SLIDER_VALUE "ATTR_SLIDER_VALUE"

/** States*/
#define STATE_HITTABLE "STATE_HITTABLE"
#define STATE_TOLEFT "STATE_TOLEFT"
#define STATE_TORIGHT "STATE_TORIGHT"
#define STATE_HIT "STATE_HIT"
#define STATE_COLLID_SOURCE "STATE_COLLID_SOURCE"
#define STATE_COLLID_TARGET "STATE_COLLID_TARGET"
#define STATE_DISABLED "STATE_DISABLED"
#define STATE_ENABLED "STATE_ENABLED"
#define STATE_LOCKED "STATE_LOCKED"
#define STATE_SELECTED "STATE_SELECTED"

/** Net messages */
#define NET_MSG_UPDATE "NET_MSG_UPDATE"


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