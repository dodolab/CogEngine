#pragma once

#include <string>
#include "GMsg.h"
#include <vector>
#include "EnInputAct.h"
#include "ofBaseTypes.h"
#include "ofxSmartPointer.h"
#include <functional>
#include "EnSound.h"
#include "ofxXmlSettings.h"
#include "EnAnim.h"

class GBehavior;
using namespace std;


/*************************************************************************/
/* Facade methods that are used for centralized access to all components */
/*************************************************************************/

// =================== MENVIRONMENT ====================
void COGAddSound(spt<EnSound> sound);
void COGPlaySound(spt<EnSound> sound);
vector<EnInputAct>& COGGetPressedKeys();
vector<EnInputAct>& COGGetPressedPoints();
vector<spt<EnSound>>& COGGetPlayedSounds();
int COGGetScreenWidth();
int COGGetScreenHeight();
ofVec2f COGGetScreenSize();
void COGRunThread(ofThread* thread);

// =================== MFACTORY ========================

float COGTranslateSpeed(float speed);

// =================== MREPOSITORY ========================

void COGRegisterListener(int action, GBehavior* beh);
void COGUnregisterListener(int action, GBehavior* beh);
void COGSendMessage(GMsg& msg, GNode* actualNode);
void COGSendDirectMessageToBehavior(GMsg& msg, int targetId);
GNode* COGFindGameObjectById(int id);
int COGGetGameObjectsCountByTag(string tag);
GNode* COGFindGameObjectByTag(string tag);
vector<GNode*> COGFindGameObjectsByTag(char* tag);
int COGGetGameObjectsCountBySubType(int subtype);
GNode* COGFindGameObjectBySubType(int subtype);
vector<GNode*> COGFindGameObjectsBySubType(int subtype);
bool COGAddGameObject(GNode* gameObject);
void COGRemoveGameObject(GNode* gameObject);
bool COGAddBehavior(GBehavior* beh);
void COGRemoveBehavior(GBehavior* beh);

// =================== MLOGGER =========================

void COGLogError(const char* format, ...);
void COGLogInfo(const char* format, ...);
void COGLogDebug(const char* format, ...);
void COGLoggerFlush();

// =================== MSTORAGE =======================

spt<ofImage> COGGet2DImage(string path);
spt<ofImage> COGPreload2DImage(string path);
spt<ofVboMesh> COGGetMesh(string path);
spt<ofTrueTypeFont> COGGetFont(string path, int size);
spt<EnSound> COGGetSound(string path);
spt<ofxXmlSettings> COGPreloadXMLFile(string path);
spt<ofxXmlSettings> COGLoadXMLFile(string path);
spt<EnAnim> COGGetAnimation(string name);
void COGStoreAnimation(spt<EnAnim> anim);
