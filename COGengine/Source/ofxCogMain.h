#pragma once

// disable third-party warnings
#pragma warning(push, 0)        

// include STD

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <assert.h>
#include <memory>    
#include <cmath>
#include <functional>
#include <stack>

using namespace std;

// include openFrameworks

#ifdef ANDROID
#include "ofxAndroid.h"
#include "ofxAndroidVibrator.h"
#endif

#include "ofUtils.h"
#include "ofBaseTypes.h"
#include "pugixml.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

using namespace pugi; // xml library

#pragma warning(pop)



// include CogEngine

#include "CogConfig.h"

#include "Constants.h"
#include "Error.h"
#include "TimeMeasure.h"
#include "Definitions.h"
#include "Utils.h"
#include "StrId.h"
#include "Flags.h"
#include "Vec2i.h"
#include "Transform.h"
#include "Facade.h"
#include "ofxCogApp.h"
#include "Node.h"
#include "Scene.h"
#include "HitEvent.h"
#include "ofxCogEngine.h"
#include "Resources.h"
#include "Logger.h"
#include "Stage.h"
#include "Tween.h"
#include "SceneSwitchManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "ComponentStorage.h"
#include "InputHandler.h"

using namespace Cog;

