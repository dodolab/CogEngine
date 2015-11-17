#pragma once

#include "ofxCogMain.h"
#include "CogMsg.h"
#include "CogInputAct.h"
#include "CogSoundFile.h"
#include "CogAnim.h"

class CogBehavior;


/*************************************************************************/
/* Facade methods that are used for centralized access to all components */
/*************************************************************************/

// =================== MENVIRONMENT ====================
void CogAddSound(spt<CogSound> sound);
void CogPlaySound(spt<CogSound> sound);
vector<CogInputAct>& CogGetPressedKeys();
vector<CogInputAct>& CogGetPressedPoints();
vector<spt<CogSound>>& CogGetPlayedSounds();
int CogGetScreenRealWidth();
int CogGetScreenRealHeight();
int CogGetScreenWidth();
int CogGetScreenHeight();
float CogGetScreenRealAspectRatio();
float CogGetScreenAspectRatio();
void CogSetScreenAspectRatio(float ratio);
ofVec2f CogGetScreenSize();
void CogRunThread(ofThread* thread);

// =================== MFACTORY ========================

float CogTranslateSpeed(float speed);

// =================== MREPOSITORY ========================

void CogRegisterListener(int action, CogBehavior* beh);
void CogUnregisterListener(int action, CogBehavior* beh);
void CogSendMessage(CogMsg& msg, CogNode* actualNode);
void CogSendDirectMessageToBehavior(CogMsg& msg, int targetId);
CogNode* CogFindNodeById(int id);
int CogGetNodesCountByTag(string tag);
CogNode* CogFindNodeByTag(string tag);
vector<CogNode*> CogFindNodesByTag(char* tag);
int CogGetNodesCountBySubType(int subtype);
CogNode* CogFindNodeBySubType(int subtype);
vector<CogNode*> CogFindNodesBySubType(int subtype);
bool CogAddNode(CogNode* node);
void CogRemoveNode(CogNode* node);
bool CogAddBehavior(CogBehavior* beh);
void CogRemoveBehavior(CogBehavior* beh);

// =================== MLOGGER =========================

void CogLogError(const char* module, const char* format, ...);
void CogLogInfo(const char* module, const char* format, ...);
void CogLogDebug(const char* module, const char* format, ...);
void CogLoggerFlush();

// =================== MRENDERER =========================
void CogPushNodeForRendering(CogNode* node);
void CogRender();

// =================== MSTORAGE =======================

spt<ofImage> CogGet2DImage(string path);
spt<ofImage> CogPreload2DImage(string path);
spt<ofVboMesh> CogGetMesh(string path);
spt<ofTrueTypeFont> CogGetFont(string path, int size);
spt<CogSound> CogGetSound(string path);
spt<ofxXmlSettings> CogPreloadXMLFile(string path);
spt<ofxXmlSettings> CogLoadXMLFile(string path);
spt<CogAnim> CogGetAnimation(string name);
void CogStoreAnimation(spt<CogAnim> anim);
