
#include "MFacade.h"
#include "MGameEngine.h"
#include "MLogger.h"
#include "MGameStorage.h"

// =================== MENVIRONMENT ====================

void COGAddSound(EnSound* sound){
	MEngine.environmentCtrl->AddSound(sound);
}

vector<EnInputAct>& COGGetPressedKeys(){
	return MEngine.environmentCtrl->GetPressedKeys();
}

vector<EnInputAct>& COGGetPressedPoints(){
	return MEngine.environmentCtrl->GetPressedPoints();
}

vector<EnSound*>& COGGetPlayedSounds(){
	return MEngine.environmentCtrl->GetPlayedSounds();
}

int COGGetScreenWidth(){
	return MEngine.environmentCtrl->GetWidth();
}

int COGGetScreenHeight(){
	return MEngine.environmentCtrl->GetHeight();
}

ofVec2f COGGetScreenSize(){
	return MEngine.environmentCtrl->GetSize();
}

// =================== MFACTORY ========================

float COGTranslateSpeed(float speed){
	return MEngine.factory->TranslateSpeed(speed);
}

// =================== MSTORAGE ========================

void COGRegisterListener(int action, GBehavior* beh){
	MEngine.storage->RegisterListener(action, beh);
}

void COGUnregisterListener(int action, GBehavior* beh){
	MEngine.storage->UnregisterListener(action, beh);
}

void COGSendMessage(GMsg& msg, GNode* actualNode){
	COGLogDebug("Message %s:%s:%d", Actions::ToString(msg.GetAction()), actualNode->GetTag().c_str(), actualNode->GetSubType());
	MEngine.storage->SendMessage(msg, actualNode);
}

void COGSendDirectMessageToBehavior(GMsg& msg, int targetId){
	MEngine.storage->SendDirectMessageToBehavior(msg, targetId);
}


GNode* COGFindGameObjectById(int id) {
	return MEngine.storage->FindGameObjectById(id);
}

int COGGetGameObjectsCountByTag(string tag) {
	return MEngine.storage->GetGameObjectsCountByTag(tag);
}

GNode* COGFindGameObjectByTag(string tag) {
	return MEngine.storage->FindGameObjectByTag(tag);
}

vector<GNode*> COGFindGameObjectsByTag(char* tag) {
	return MEngine.storage->FindGameObjectsByTag(tag);
}

int COGGetGameObjectsCountBySubType(int subtype) {
	return MEngine.storage->GetGameObjectsCountBySubType(subtype);
}

GNode* COGFindGameObjectBySubType(int subtype) {
	return MEngine.storage->FindGameObjectBySubType(subtype);
}

vector<GNode*> COGFindGameObjectsBySubType(int subtype) {
	return MEngine.storage->FindGameObjectsBySubType(subtype);
}

bool COGAddGameObject(GNode* gameObject){
	MLOGDEBUG("Adding gameobject %s", gameObject->GetTag().c_str());
	return MEngine.storage->AddGameObject(gameObject);
}

void COGRemoveGameObject(GNode* gameObject){
	MLOGDEBUG("Removing gameobject %s", gameObject->GetTag().c_str());
	MEngine.storage->RemoveGameObject(gameObject);
}

bool COGAddBehavior(GBehavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "Behavior %s hasn't game object assigned", typeid(*beh).name());
	MLOGDEBUG("Adding behavior %s to gameobject %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	return MEngine.storage->AddBehavior(beh);
}

void COGRemoveBehavior(GBehavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "Behavior %s hasn't game object assigned", typeid(*beh).name());
	MLOGDEBUG("Removing behavior %s from gameobject %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	MEngine.storage->RemoveBehavior(beh);
}

// =================== MLOGGER =========================

void COGLogError(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogError(0, format, args);
	va_end(args);
}

void COGLogInfo(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogInfo(0, format, args);
	va_end(args);
}

void COGLogDebug(const char* format, ...){
	va_list args;
	va_start(args, format);
	MEngine.logger->LogDebug(0, format, args);
	va_end(args);
}

void COGLoggerFlush(){
	MEngine.logger->Flush();
}

// =================== MRESOURCE =======================

spt<ofImage> COGGet2DImage(string name){
	return MEngine.resourceCtrl->Get2DImage(name);
}

spt<ofVboMesh> COGGetMesh(string name){
	return MEngine.resourceCtrl->GetMesh(name);
}

spt<ofTrueTypeFont> COGGetFont(string name, int size){
	return MEngine.resourceCtrl->GetFont(name, size);
}