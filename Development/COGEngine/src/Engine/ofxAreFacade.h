#pragma once

#include "ofxAreMain.h"
#include "ofxAreMsg.h"
#include "ofxAraInputAct.h"
#include "ofBaseTypes.h"
#include "ofxAraSound.h"
#include "ofxAraAnim.h"

class ofxAreBehavior;
using namespace std;


/*************************************************************************/
/* Facade methods that are used for centralized access to all components */
/*************************************************************************/

// =================== MENVIRONMENT ====================
void COGAddSound(spt<ofxAraSound> sound);
void COGPlaySound(spt<ofxAraSound> sound);
vector<ofxAraInputAct>& COGGetPressedKeys();
vector<ofxAraInputAct>& COGGetPressedPoints();
vector<spt<ofxAraSound>>& COGGetPlayedSounds();
int COGGetScreenWidth();
int COGGetScreenHeight();
ofVec2f COGGetScreenSize();
void COGRunThread(ofThread* thread);

// =================== MFACTORY ========================

float COGTranslateSpeed(float speed);

// =================== MREPOSITORY ========================

void COGRegisterListener(int action, ofxAreBehavior* beh);
void COGUnregisterListener(int action, ofxAreBehavior* beh);
void COGSendMessage(ofxAreMsg& msg, ofxAreNode* actualNode);
void COGSendDirectMessageToBehavior(ofxAreMsg& msg, int targetId);
ofxAreNode* COGFindNodeById(int id);
int COGGetNodesCountByTag(string tag);
ofxAreNode* COGFindNodeByTag(string tag);
vector<ofxAreNode*> COGFindNodesByTag(char* tag);
int COGGetNodesCountBySubType(int subtype);
ofxAreNode* COGFindNodeBySubType(int subtype);
vector<ofxAreNode*> COGFindNodesBySubType(int subtype);
bool COGAddNode(ofxAreNode* node);
void COGRemoveNode(ofxAreNode* node);
bool COGAddBehavior(ofxAreBehavior* beh);
void COGRemoveBehavior(ofxAreBehavior* beh);

// =================== MLOGGER =========================

void COGLogError(const char* module, const char* format, ...);
void COGLogInfo(const char* module, const char* format, ...);
void COGLogDebug(const char* module, const char* format, ...);
void COGLoggerFlush();

// =================== MRENDERER =========================
void COGPushNodeForRendering(ofxAreNode* node);
void COGRender();

// =================== MSTORAGE =======================

spt<ofImage> COGGet2DImage(string path);
spt<ofImage> COGPreload2DImage(string path);
spt<ofVboMesh> COGGetMesh(string path);
spt<ofTrueTypeFont> COGGetFont(string path, int size);
spt<ofxAraSound> COGGetSound(string path);
spt<ofxXmlSettings> COGPreloadXMLFile(string path);
spt<ofxXmlSettings> COGLoadXMLFile(string path);
spt<ofxAraAnim> COGGetAnimation(string name);
void COGStoreAnimation(spt<ofxAraAnim> anim);
