#ifndef OFX_COG_MAIN
#define OFX_COG_MAIN

// disable third-party warnings
#pragma warning(push, 0)        

#ifdef ANDROID
#include "ofxAndroid.h"
#include "ofxAndroidVibrator.h"
#endif

#include "ofBaseTypes.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "CogEnums.h"
#include "ofxSmartPointer.h"
#include "ofSoundPlayer.h"
#include "ofMain.h"
#include "ofxSQLite.h"
#include "ofxXmlSettings.h"
#include "CogError.h"
#include <functional>
#include <list>
#include <stdexcept>
#include <assert.h>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <cmath>
#include <functional>
#include "ofImage.h"
#include "ofTrueTypeFont.h"
#include "ofUtils.h"
#include "CollageTexture.h"
#include "SpriteTexture.h"
#include "SQLiteEntity.h"
#include "SpriteSheetRenderer.h"

#pragma warning(pop)

using namespace std;

#endif