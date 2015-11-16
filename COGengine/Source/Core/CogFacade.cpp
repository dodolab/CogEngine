
#include "CogFacade.h"
#include "CogEngine.h"
#include "Logger.h"
#include "Cache.h"

// =================== MENVIRONMENT ====================

void COGAddSound(spt<SoundFile> sound){
	COGEngine.environmentCtrl->AddSound(sound);
}

void COGPlaySound(spt<SoundFile> sound){
	COGEngine.environmentCtrl->PlaySound(sound);
}

vector<InputAct>& COGGetPressedKeys(){
	return COGEngine.environmentCtrl->GetPressedKeys();
}

vector<InputAct>& COGGetPressedPoints(){
	return COGEngine.environmentCtrl->GetPressedPoints();
}

vector<spt<SoundFile>>& COGGetPlayedSounds(){
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

void COGRegisterListener(int action, Behavior* beh){
	COGEngine.storage->RegisterListener(action, beh);
}

void COGUnregisterListener(int action, Behavior* beh){
	COGEngine.storage->UnregisterListener(action, beh);
}

void COGSendMessage(Msg& msg, Node* actualNode){
	MLOGDEBUG("CORE", "Message %s:%s:%d", ofToString(msg.GetAction()).c_str(), actualNode->GetTag().c_str(), actualNode->GetSubType());
	COGEngine.storage->SendMessage(msg, actualNode);
}

void COGSendDirectMessageToBehavior(Msg& msg, int targetId){
	COGEngine.storage->SendDirectMessageToBehavior(msg, targetId);
}


Node* COGFindNodeById(int id) {
	return COGEngine.storage->FindNodeById(id);
}

int COGGetNodesCountByTag(string tag) {
	return COGEngine.storage->GetNodesCountByTag(tag);
}

Node* COGFindNodeByTag(string tag) {
	return COGEngine.storage->FindNodeByTag(tag);
}

vector<Node*> COGFindNodesByTag(char* tag) {
	return COGEngine.storage->FindNodesByTag(tag);
}

int COGGetNodesCountBySubType(int subtype) {
	return COGEngine.storage->GetNodesCountBySubType(subtype);
}

Node* COGFindNodeBySubType(int subtype) {
	return COGEngine.storage->FindNodeBySubType(subtype);
}

vector<Node*> COGFindNodesBySubType(int subtype) {
	return COGEngine.storage->FindNodesBySubType(subtype);
}

bool COGAddNode(Node* node){
	MLOGDEBUG("CORE", "Adding node %s", node->GetTag().c_str());
	return COGEngine.storage->AddNode(node);
}

void COGRemoveNode(Node* node){
	MLOGDEBUG("CORE", "Removing node %s", node->GetTag().c_str());
	COGEngine.storage->RemoveNode(node);
}

bool COGAddBehavior(Behavior* beh){
	MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
	MLOGDEBUG("CORE", "Adding behavior %s to node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
	return COGEngine.storage->AddBehavior(beh);
}

void COGRemoveBehavior(Behavior* beh){
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
void COGPushNodeForRendering(Node* node){
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

spt<SoundFile> COGGetSound(string path){
	return COGEngine.resourceCtrl->GetSound(path);
}

spt<ofxXmlSettings> COGPreloadXMLFile(string path){
	return COGEngine.resourceCtrl->PreloadXMLFile(path);
}

spt<ofxXmlSettings> COGLoadXMLFile(string path){
	return COGEngine.resourceCtrl->LoadXMLFile(path);
}

spt<Anim> COGGetAnimation(string name){
	return COGEngine.resourceCtrl->GetAnimation(name);
}

void COGStoreAnimation(spt<Anim> anim){
	COGEngine.resourceCtrl->StoreAnimation(anim);
}