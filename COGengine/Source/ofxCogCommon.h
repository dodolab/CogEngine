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
#include <stdexcept>
#include <assert.h>
#include <stdarg.h>  
#include <memory>    
#include <cmath>
#include <functional>
#include <stack>
#include "Enums.h"
#include "ofxSmartPointer.h"
#include "ofSoundPlayer.h"
#include "ofMain.h"
#include "ofxSQLite.h"
#include "ofxXmlSettings.h"
#include "Error.h"
#include "ofImage.h"
#include "ofTrueTypeFont.h"
#include "CollageTexture.h"
#include "SpriteTexture.h"
#include "SQLiteEntity.h"
#include "SpriteSheetRenderer.h"
#include "Definitions.h"
#include "Utils.h"
#include "StringHash.h"
#include "Flags.h"
#include "Vec2i.h"

#pragma warning(pop)



using namespace std;

