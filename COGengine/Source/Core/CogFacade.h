#pragma once

#include "ofxCogMain.h"
#include "Msg.h"
#include "InputAct.h"
#include "SoundFile.h"
#include "Anim.h"

class Behavior;


/*************************************************************************/
/* Facade methods that are used for centralized access to all components */
/*************************************************************************/

// =================== MENVIRONMENT ====================
void COGAddSound(spt<SoundFile> sound);
void COGPlaySound(spt<SoundFile> sound);
vector<InputAct>& COGGetPressedKeys();
vector<InputAct>& COGGetPressedPoints();
vector<spt<SoundFile>>& COGGetPlayedSounds();
int COGGetScreenRealWidth();
int COGGetScreenRealHeight();
int COGGetScreenWidth();
int COGGetScreenHeight();
float COGGetScreenRealAspectRatio();
float COGGetScreenAspectRatio();
void COGSetScreenAspectRatio(float ratio);
ofVec2f COGGetScreenSize();
void COGRunThread(ofThread* thread);

// =================== MFACTORY ========================

float COGTranslateSpeed(float speed);

// =================== MREPOSITORY ========================

void COGRegisterListener(int action, Behavior* beh);
void COGUnregisterListener(int action, Behavior* beh);
void COGSendMessage(Msg& msg, Node* actualNode);
void COGSendDirectMessageToBehavior(Msg& msg, int targetId);
Node* COGFindNodeById(int id);
int COGGetNodesCountByTag(string tag);
Node* COGFindNodeByTag(string tag);
vector<Node*> COGFindNodesByTag(char* tag);
int COGGetNodesCountBySubType(int subtype);
Node* COGFindNodeBySubType(int subtype);
vector<Node*> COGFindNodesBySubType(int subtype);
bool COGAddNode(Node* node);
void COGRemoveNode(Node* node);
bool COGAddBehavior(Behavior* beh);
void COGRemoveBehavior(Behavior* beh);

// =================== MLOGGER =========================

void COGLogError(const char* module, const char* format, ...);
void COGLogInfo(const char* module, const char* format, ...);
void COGLogDebug(const char* module, const char* format, ...);
void COGLoggerFlush();

// =================== MRENDERER =========================
void COGPushNodeForRendering(Node* node);
void COGRender();

// =================== MSTORAGE =======================

spt<ofImage> COGGet2DImage(string path);
spt<ofImage> COGPreload2DImage(string path);
spt<ofVboMesh> COGGetMesh(string path);
spt<ofTrueTypeFont> COGGetFont(string path, int size);
spt<SoundFile> COGGetSound(string path);
spt<ofxXmlSettings> COGPreloadXMLFile(string path);
spt<ofxXmlSettings> COGLoadXMLFile(string path);
spt<Anim> COGGetAnimation(string name);
void COGStoreAnimation(spt<Anim> anim);
