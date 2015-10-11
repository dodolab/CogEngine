#ifndef OFX_ARE_MAIN
#define OFX_ARE_MAIN

// disable third-party warnings
#pragma warning(push, 0)        

#ifdef TARGET_ANDROID
#include "ofxAndroid.h"
#include "ofxAndroidVibrator.h"
#endif

#include "ofBaseTypes.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "ofxAreEnums.h"
#include "ofxSmartPointer.h"
#include "ofSoundPlayer.h"
#include "ofMain.h"
#include "ofxSQLite.h"
#include "ofxXmlSettings.h"
#include "ofxAreError.h"
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
#include "ofxCollageTexture.h"
#include "ofxSpriteTexture.h"
#include "ofxSQLiteEntity.h"
#include "ofxSpriteSheetRenderer.h"

#pragma warning(pop)

using namespace std;

#endif