
#include "Facade.h"
#include "CogEngine.h"
#include "Logger.h"
#include "ResourceCache.h"

namespace Cog {

	// =================== MENVIRONMENT ====================

	void CogAddSound(spt<Sound> sound) {
		COGEngine.environment->AddSound(sound);
	}

	void CogPlaySound(spt<Sound> sound) {
		COGEngine.environment->PlaySound(sound);
	}

	vector<InputAct>& CogGetPressedKeys() {
		return COGEngine.environment->GetPressedKeys();
	}

	vector<InputAct>& CogGetPressedPoints() {
		return COGEngine.environment->GetPressedPoints();
	}

	vector<spt<Sound>>& CogGetPlayedSounds() {
		return COGEngine.environment->GetPlayedSounds();
	}

	int CogGetScreenRealWidth() {
		return COGEngine.environment->GetRealWidth();
	}

	int CogGetScreenRealHeight() {
		return COGEngine.environment->GetRealHeight();
	}

	int CogGetScreenWidth() {
		return COGEngine.environment->GetWidth();
	}

	int CogGetScreenHeight() {
		return COGEngine.environment->GetHeight();
	}

	float CogGetScreenRealAspectRatio() {
		return COGEngine.environment->GetRealAspectRatio();
	}

	float CogGetScreenAspectRatio() {
		return COGEngine.environment->GetAspectRatio();
	}

	void CogSetScreenAspectRatio(float ratio) {
		COGEngine.environment->SetAspectRatio(ratio);
	}



	ofVec2f CogGetScreenSize() {
		return COGEngine.environment->GetSize();
	}

	void CogRunThread(ofThread* thread) {
		COGEngine.environment->RunThread(thread);
	}

	// =================== MFACTORY ========================

	float CogTranslateSpeed(float speed) {
		// todo: 
		return speed * 0.001f*COGEngine.environment->GetWidth();
	}

	// =================== MSTORAGE ========================

	void CogRegisterListener(StringHash action, MsgListener* listener) {
		COGEngine.sceneContext->RegisterListener(action, listener);
	}

	void CogUnregisterListener(StringHash action, MsgListener* listener) {
		COGEngine.sceneContext->UnregisterListener(action, listener);
	}

	void CogSendMessage(Msg& msg, Node* actualNode) {
		MLOGDEBUG("CORE", "Message %s:%s:%d", ofToString(msg.GetAction()).c_str(), actualNode->GetTag().c_str(), actualNode->GetSubType());
		COGEngine.sceneContext->SendMessage(msg, actualNode);
	}

	void CogSendDirectMessageToListener(Msg& msg, int targetId) {
		COGEngine.sceneContext->SendDirectMessageToListener(msg, targetId);
	}

	void CogSendDirectMessage(StringHash action, int subaction, MsgEvent* data, Node* source, int listenerId) {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, listenerId, source, data);
		CogSendMessage(msg, source);
	}

	void CogSendDirectMessageToListener(StringHash action, int subaction, MsgEvent* data, Node* source, int targetId, int listenerId) {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, listenerId, source, data);
		CogSendDirectMessageToListener(msg, targetId);
	}

	Node* CogFindNodeById(int id) {
		return COGEngine.sceneContext->FindNodeById(id);
	}

	int CogGetNodesCountByTag(string tag) {
		return COGEngine.sceneContext->GetNodesCountByTag(tag);
	}

	Node* CogFindNodeByTag(string tag) {
		return COGEngine.sceneContext->FindNodeByTag(tag);
	}

	vector<Node*> CogFindNodesByTag(char* tag) {
		return COGEngine.sceneContext->FindNodesByTag(tag);
	}

	int CogGetNodesCountBySubType(int subtype) {
		return COGEngine.sceneContext->GetNodesCountBySubType(subtype);
	}

	Node* CogFindNodeBySubType(int subtype) {
		return COGEngine.sceneContext->FindNodeBySubType(subtype);
	}

	vector<Node*> CogFindNodesBySubType(int subtype) {
		return COGEngine.sceneContext->FindNodesBySubType(subtype);
	}

	bool CogAddNode(Node* node) {
		MLOGDEBUG("CORE", "Adding node %s", node->GetTag().c_str());
		return COGEngine.sceneContext->AddNode(node);
	}

	void CogRemoveNode(Node* node) {
		MLOGDEBUG("CORE", "Removing node %s", node->GetTag().c_str());
		COGEngine.sceneContext->RemoveNode(node);
	}

	bool CogAddBehavior(Behavior* beh) {
		MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
		MLOGDEBUG("CORE", "Adding behavior %s to node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
		return COGEngine.sceneContext->AddBehavior(beh);
	}

	void CogRemoveBehavior(Behavior* beh) {
		MASSERT(beh->GetOwner() != nullptr, "CORE", "Behavior %s hasn't node assigned", typeid(*beh).name());
		MLOGDEBUG("CORE", "Removing behavior %s from node %s", typeid(*beh).name(), beh->GetOwner()->GetTag().c_str());
		COGEngine.sceneContext->RemoveBehavior(beh);
	}

	// =================== MLOGGER =========================

	void CogLogError(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		COGEngine.logger->LogError(module, 0, format, args);
		va_end(args);
	}

	void CogLogInfo(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		COGEngine.logger->LogInfo(module, 0, format, args);
		va_end(args);
	}

	void CogLogDebug(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		COGEngine.logger->LogDebug(module, 0, format, args);
		va_end(args);
	}

	void CogLoggerFlush() {
		COGEngine.logger->Flush();
	}


	// =================== MRENDERER =========================
	void CogPushNodeForRendering(Node* node) {
		COGEngine.renderer->PushNode(node);
	}

	void CogRender() {
		COGEngine.renderer->Render();
	}

	// =================== MCACHE =======================

	spt<ofImage> CogGet2DImage(string path) {
		return COGEngine.resourceCache->Get2DImage(path);
	}

	spt<ofImage> CogPreload2DImage(string path) {
		return COGEngine.resourceCache->Preload2DImage(path);
	}

	spt<ofVboMesh> CogGetMesh(string path) {
		return COGEngine.resourceCache->GetMesh(path);
	}

	spt<ofTrueTypeFont> CogGetFont(string path, int size) {
		return COGEngine.resourceCache->GetFont(path, size);
	}

	spt<Sound> CogGetSound(string path) {
		return COGEngine.resourceCache->GetSound(path);
	}

	spt<ofxXmlSettings> CogPreloadXMLFile(string path) {
		return COGEngine.resourceCache->PreloadXMLFile(path);
	}

	spt<ofxXmlSettings> CogLoadXMLFile(string path) {
		return COGEngine.resourceCache->LoadXMLFile(path);
	}

	spt<Anim> CogGetAnimation(string name) {
		return COGEngine.resourceCache->GetAnimation(name);
	}

	void CogStoreAnimation(spt<Anim> anim) {
		COGEngine.resourceCache->StoreAnimation(anim);
	}


	// ================== MCOMPONENTSTORAGE ====================

	void CogRegisterComponent(Component* value) {
		COGEngine.entityStorage->RegisterComponent(value);
	}

	bool CogRemoveComponent(StringHash key) {
		return COGEngine.entityStorage->RemoveComponent(key);
	}

	bool CogExistsComponent(StringHash key) {
		return COGEngine.entityStorage->ExistsComponent(key);
	}

}// namespace