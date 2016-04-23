#pragma once

// disable third-party warnings
#pragma warning(push, 0)        

#ifdef ANDROID
#include "ofxAndroid.h"
#include "ofxAndroidVibrator.h"
#endif

#include "ofUtils.h"
#include "ofBaseTypes.h"
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
#include "Constants.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Error.h"
#include "TimeMeasure.h"
#include "Definitions.h"
#include "Utils.h"
#include "StrId.h"
#include "Flags.h"
#include "Vec2i.h"
#include "Transform.h"
#include "Facade.h"
#include "ofMain.h"

#pragma warning(pop)


#include "ofxCogApp.h"
#include "Node.h"
#include "Scene.h"
#include "HitEvent.h"
#include "MapLoader.h"
#include "Settings.h"
#include "AStarSearch.h"
#include "TransformAnim.h"

using namespace std;
using namespace Cog;

#include "ofxCogEngine.h"
#include "ResourceCache.h"
#include "Logger.h"
#include "Stage.h"
#include "Tween.h"
#include "SceneSwitchManager.h"
#include "Renderer.h"
#include "Environment.h"
#include "ComponentStorage.h"
#include "InputHandler.h"