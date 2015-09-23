
#include "MFacade.h"
#include "MEngine.h"
#include "MLogger.h"
#include "MRepository.h"

// =================== MENVIRONMENT ====================

void COGAddSound(spt<EnSound> sound){
	COGEngine.environmentCtrl->AddSound(sound);
}

void COGPlaySound(spt<EnSound> sound){
	COGEngine.environmentCtrl->PlaySound(sound);
}

vector<EnInputAct>& COGGetPressedKeys(){
	return COGEngine.environmentCtrl->GetPressedKeys();
}

vector<EnInputAct>& COGGetPressedPoints(){
	return COGEngine.environmentCtrl->GetPressedPoints();
}

vector<spt<EnSound>>& COGGetPlayedSounds(){
	return COGEngine.environmentCtrl->GetPlayedSounds();
}

int COGGetScreenWidth(){
	return COGEngine.environmentCtrl->GetWidth();
}

int COGGetScreenHeight(){
	return COGEngine.environmentCtrl->GetHeight();
}

ofVec2f COGGetScreenSize(){
	return COGEngine.environmentCtrl->GetSize();
}

void COGRunThread(ofThread* thread){
	COGEngine.environmentCtrl->RunThread(thread);
}

// =================== MFACTORY ========================

float COGTranslateSpeed(float speed){
	return COGEngine.factory->TranslateSpeed(speed);
}

// =================== MSTORAGE ========================

void COGRegisterListener(int action, GBehavior* beh){
	COGEngine.storage->RegisterListener(action, beh);
}

void COGUnregisterListener(int action, GBehavior* beh){
	COGEngine.storage->UnregisterListener(action, beh);
}

void COGSendMessage(GMsg& msg, GNode* actualNode){
	COGLogDebug("CORE", "Message %s:%s:%d", Actions::ToString(msg.GetAction()), actualNode->GetTag().c_str(), actualNode->GetSubType());
	COGEngine.storage->SendMessage(msg, actualNode);
}

void COGSendDirectMessageToBehavior(GMsg& msg, int targetId){
	COGEngine.storage->SendDirectMessageToBehavior(msg, targetId);
}


GNode* COGFindNodeById(int id) {
	return COGEngine.storage->FindNodeById(id);
}

int COGGetNodesCountByTag(string tag) {
	return COGEngine.storage->GetNodesCountByTag(tag);
}

GNode* COGFindNodeByTag(string tag) {
	return COGEngine.storage->FindNodeByTag(tag);
}

vector<GNode*> COGFindNodesByTag(char* tag) {
	return COGEngine.storage->FindNodesByTag(tag);
}

int COGGetNodesCountBySubType(int subtype) {
	return COGEngine.storage->GetNodesCountBySubType(subtype);
}

GNode* COGFindNodeBySubType(int subtype) {
	return COGEngine.storage->FindNodeBySubType(subtype);
}

vector<GNode*> COGFindNodesBySubType(int subtype) {
	return COGEngine.storage->FindNodesBySubType(subtype);
}

bool COGAddNode(GNode* node){
	MLOGDEBUG("CORE", "Adding node %s", node->GetTag().c_str());
	return COGEngine.storage->AddNode(node);
}

void COGRemoveNode(GNode* node){
	MLOGDEBUG("CORE", "Removing node %s", node->GetTag().c_str());
	COGEngine.storage->RemoveNode(node);
}

bool COGAddBehavior(GBehavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
	MLOGDEBUG("CORE", "Adding behavior %s to node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	return COGEngine.storage->AddBehavior(beh);
}

void COGRemoveBehavior(GBehavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
	MLOGDEBUG("CORE", "Removing behavior %s from node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	COGEngine.storage->RemoveBehavior(beh);
}

// =================== MLOGGER =========================

void COGLogError(const char* module, const char* format, ...){
	va_list args;
	va_start(args, format);
	COGEngine.logger->LogError(module, 0, format, args);
	va_end(args);
}

void COGLogInfo(const char* module, const char* format, ...){
	va_list args;
	va_start(args, format);
	COGEngine.logger->LogInfo(module, 0, format, args);
	va_end(args);
}

void COGLogDebug(const char* module, const char* format, ...){
	va_list args;
	va_start(args, format);
	COGEngine.logger->LogDebug(module, 0, format, args);
	va_end(args);
}

void COGLoggerFlush(){
	COGEngine.logger->Flush();
}

// =================== MRESOURCE =======================

spt<ofImage> COGGet2DImage(string path){
	return COGEngine.resourceCtrl->Get2DImage(path);
}

spt<ofImage> COGPreload2DImage(string path){
	return COGEngine.resourceCtrl->Preload2DImage(path);
}

spt<ofVboMesh> COGGetMesh(string path){
	return COGEngine.resourceCtrl->GetMesh(path);
}

spt<ofTrueTypeFont> COGGetFont(string path, int size){
	return COGEngine.resourceCtrl->GetFont(path, size);
}

spt<EnSound> COGGetSound(string path){
	return COGEngine.resourceCtrl->GetSound(path);
}

spt<ofxXmlSettings> COGPreloadXMLFile(string path){
	return COGEngine.resourceCtrl->PreloadXMLFile(path);
}

spt<ofxXmlSettings> COGLoadXMLFile(string path){
	return COGEngine.resourceCtrl->LoadXMLFile(path);
}

spt<EnAnim> COGGetAnimation(string name){
	return COGEngine.resourceCtrl->GetAnimation(name);
}

void COGStoreAnimation(spt<EnAnim> anim){
	COGEngine.resourceCtrl->StoreAnimation(anim);
}