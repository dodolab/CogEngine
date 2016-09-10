
#include "Facade.h"
#include "CogEngine.h"
#include "Logger.h"
#include "ResourceCache.h"

namespace Cog {

	EntityStorage* CogGetEntityStorage() {
		return COGEngine.entityStorage;
	}

	// =================== MENVIRONMENT ====================

	void CogAddSound(spt<Sound> sound) {
		COGEngine.environment->AddSound(sound);
	}

	void CogPlaySound(spt<Sound> sound) {
		COGEngine.environment->PlaySound(sound);
	}

	vector<InputAct*>& CogGetPressedKeys() {
		return COGEngine.environment->GetPressedKeys();
	}

	vector<InputAct*>& CogGetPressedPoints() {
		return COGEngine.environment->GetPressedPoints();
	}

	vector<spt<Sound>>& CogGetPlayedSounds() {
		return COGEngine.environment->GetPlayedSounds();
	}

	bool CogIsKeyPressed(int key) {
		return COGEngine.environment->IsKeyPressed(key);
	}

	Vec2i CogGetMousePosition() {
		return COGEngine.environment->GetMousePosition();
	}

	int CogGetScreenWidth() {
		return COGEngine.environment->GetScreenWidth();
	}

	int CogGetVirtualWidth() {
		return COGEngine.environment->GetVirtualWidth();
	}

	int CogGetScreenHeight() {
		return COGEngine.environment->GetScreenHeight();
	}

	int CogGetVirtualHeight() {
		return COGEngine.environment->GetVirtualHeight();
	}

	float CogGetVirtualAspectRatio() {
		return COGEngine.environment->GetVirtualAspectRatio();
	}

	float CogGetScreenAspectRatio() {
		return COGEngine.environment->GetScreenAspectRatio();
	}

	Vec2i CogGetScreenSize() {
		return COGEngine.environment->GetScreenSize();
	}

	Vec2i CogGetVirtualScreenSize() {
		return COGEngine.environment->GetVirtualScreenSize();
	}

	void CogRunThread(ofThread* thread) {
		COGEngine.environment->RunThread(thread);
	}


	// =================== MSTORAGE ========================

	void CogRegisterGlobalListener(StringHash action, MsgListener* listener) {
		COGEngine.sceneContext->RegisterListener(action, listener);
	}

	void CogUnregisterGlobalListener(StringHash action, MsgListener* listener) {
		COGEngine.sceneContext->UnregisterListener(action, listener);
	}

	void CogSendMessage(Msg& msg, Node* actualNode) {
		COGEngine.sceneContext->GetActualScene()->SendMessage(msg, actualNode);
	}

	void CogSendDirectMessageToListener(Msg& msg, int targetId) {
		COGEngine.sceneContext->GetActualScene()->SendDirectMessageToListener(msg, targetId);
	}

	void CogSendDirectMessage(StringHash action, int subaction, MsgEvent* data, Node* source, int listenerId) {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, listenerId, source, data);
		CogSendMessage(msg, source);
	}

	void CogSendDirectMessageToListener(StringHash action, int subaction, MsgEvent* data, Node* source, int targetId, int listenerId) {
		Msg msg(BubblingType(Scope::DIRECT_NO_TRAVERSE, true, true), action, subaction, listenerId, source, data);
		CogSendDirectMessageToListener(msg, targetId);
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