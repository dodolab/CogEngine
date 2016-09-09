
#include "ofxAreFacade.h"
#include "ofxAreEngine.h"
#include "ofxAreLogger.h"
#include "ofxArcRepository.h"

// =================== MENVIRONMENT ====================

void COGAddSound(spt<ofxAraSound> sound){
	COGEngine.environmentCtrl->AddSound(sound);
}

void COGPlaySound(spt<ofxAraSound> sound){
	COGEngine.environmentCtrl->PlaySound(sound);
}

vector<ofxAraInputAct>& COGGetPressedKeys(){
	return COGEngine.environmentCtrl->GetPressedKeys();
}

vector<ofxAraInputAct>& COGGetPressedPoints(){
	return COGEngine.environmentCtrl->GetPressedPoints();
}

vector<spt<ofxAraSound>>& COGGetPlayedSounds(){
	return COGEngine.environmentCtrl->GetPlayedSounds();
}

int COGGetScreenRealWidth(){
	return COGEngine.environmentCtrl->GetRealWidth();
}

int COGGetScreenRealHeight(){
	return COGEngine.environmentCtrl->GetRealHeight();
}

int COGGetScreenWidth(){
	return COGEngine.environmentCtrl->GetWidth();
}

int COGGetScreenHeight(){
	return COGEngine.environmentCtrl->GetHeight();
}

float COGGetScreenRealAspectRatio(){
	return COGEngine.environmentCtrl->GetRealAspectRatio();
}

float COGGetScreenAspectRatio(){
	return COGEngine.environmentCtrl->GetAspectRatio();
}

void COGSetScreenAspectRatio(float ratio){
	COGEngine.environmentCtrl->SetAspectRatio(ratio);
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

void COGRegisterListener(int action, ofxAreBehavior* beh){
	COGEngine.storage->RegisterListener(action, beh);
}

void COGUnregisterListener(int action, ofxAreBehavior* beh){
	COGEngine.storage->UnregisterListener(action, beh);
}

void COGSendMessage(ofxAreMsg& msg, ofxAreNode* actualNode){
	MLOGDEBUG("CORE", "Message %s:%s:%d", ofToString(msg.GetAction()).c_str(), actualNode->GetTag().c_str(), actualNode->GetSubType());
	COGEngine.storage->SendMessage(msg, actualNode);
}

void COGSendDirectMessageToBehavior(ofxAreMsg& msg, int targetId){
	COGEngine.storage->SendDirectMessageToBehavior(msg, targetId);
}


ofxAreNode* COGFindNodeById(int id) {
	return COGEngine.storage->FindNodeById(id);
}

int COGGetNodesCountByTag(string tag) {
	return COGEngine.storage->GetNodesCountByTag(tag);
}

ofxAreNode* COGFindNodeByTag(string tag) {
	return COGEngine.storage->FindNodeByTag(tag);
}

vector<ofxAreNode*> COGFindNodesByTag(char* tag) {
	return COGEngine.storage->FindNodesByTag(tag);
}

int COGGetNodesCountBySubType(int subtype) {
	return COGEngine.storage->GetNodesCountBySubType(subtype);
}

ofxAreNode* COGFindNodeBySubType(int subtype) {
	return COGEngine.storage->FindNodeBySubType(subtype);
}

vector<ofxAreNode*> COGFindNodesBySubType(int subtype) {
	return COGEngine.storage->FindNodesBySubType(subtype);
}

bool COGAddNode(ofxAreNode* node){
	MLOGDEBUG("CORE", "Adding node %s", node->GetTag().c_str());
	return COGEngine.storage->AddNode(node);
}

void COGRemoveNode(ofxAreNode* node){
	MLOGDEBUG("CORE", "Removing node %s", node->GetTag().c_str());
	COGEngine.storage->RemoveNode(node);
}

bool COGAddBehavior(ofxAreBehavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
	MLOGDEBUG("CORE", "Adding behavior %s to node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	return COGEngine.storage->AddBehavior(beh);
}

void COGRemoveBehavior(ofxAreBehavior* beh){
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


// =================== MRENDERER =========================
void COGPushNodeForRendering(ofxAreNode* node){
	COGEngine.renderer->PushNode(node);
}

void COGRender(){
	COGEngine.renderer->Render();
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

spt<ofxAraSound> COGGetSound(string path){
	return COGEngine.resourceCtrl->GetSound(path);
}

spt<ofxXmlSettings> COGPreloadXMLFile(string path){
	return COGEngine.resourceCtrl->PreloadXMLFile(path);
}

spt<ofxXmlSettings> COGLoadXMLFile(string path){
	return COGEngine.resourceCtrl->LoadXMLFile(path);
}

spt<ofxAraAnim> COGGetAnimation(string name){
	return COGEngine.resourceCtrl->GetAnimation(name);
}

void COGStoreAnimation(spt<ofxAraAnim> anim){
	COGEngine.resourceCtrl->StoreAnimation(anim);
}