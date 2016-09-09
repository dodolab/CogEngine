#pragma once

#include <string>
#include "GMsg.h"
#include <vector>
#include "EnInputAct.h"
#include "ofBaseTypes.h"
#include "SmartPointer.h"
#include <functional>

class GBehavior;
using namespace std;

// facade methods for M objects

// =================== MLOGGER =========================

void COGLogError(string message);
void COGLogInfo(string message);
void COGLogDebug(string message);
void COGLoggerSave();

// =================== MENVIRONMENT ====================

vector<EnInputAct>& COGGetPressedKeys();

vector<EnInputAct>& COGGetPressedPoints();

int COGGetWidth();

int COGGetHeight();

// =================== MRESOURCE =======================

spt<ofImage> COGGet2DImage(string name);
spt<ofVboMesh> COGGetMesh(string name);
spt<ofTrueTypeFont> COGGetFont(string name, int size);

// =================== MSTORAGE ========================


int COGRegisterCallback(int action, MsgCallback callback);
bool COGUnregisterCallback(int action, int id);
void COGRegisterListener(int action, GBehavior* beh);
void COGUnregisterListener(int action, GBehavior* beh);
void COGSendTraversationMessage(GMsg& msg, GNode* actualNode);
void COGSendMessage(GMsg& msg);
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

// =================== MENGINE ========================

float COGTranslateSpeed(float speed);