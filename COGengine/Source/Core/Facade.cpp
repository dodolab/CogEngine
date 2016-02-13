
#include "Facade.h"
#include "CogEngine.h"
#include "Logger.h"
#include "ResourceCache.h"
#include "TimeMeasure.h"

namespace Cog {

	EntityStorage* CogGetEntityStorage() {
		return CogEngine::GetInstance().entityStorage;
	}

	// =================== MENVIRONMENT ====================

	void CogAddSound(spt<Sound> sound) {
		CogEngine::GetInstance().environment->AddSound(sound);
	}

	void CogPlaySound(spt<Sound> sound) {
		CogEngine::GetInstance().environment->PlaySound(sound);
	}

	vector<InputAct*>& CogGetPressedKeys() {
		return CogEngine::GetInstance().environment->GetPressedKeys();
	}

	vector<InputAct*>& CogGetPressedPoints() {
		return CogEngine::GetInstance().environment->GetPressedPoints();
	}

	vector<spt<Sound>>& CogGetPlayedSounds() {
		return CogEngine::GetInstance().environment->GetPlayedSounds();
	}

	bool CogIsKeyPressed(int key) {
		return CogEngine::GetInstance().environment->IsKeyPressed(key);
	}

	Vec2i CogGetMousePosition() {
		return CogEngine::GetInstance().environment->GetMousePosition();
	}

	int CogGetScreenWidth() {
		return CogEngine::GetInstance().environment->GetScreenWidth();
	}

	int CogGetVirtualWidth() {
		return CogEngine::GetInstance().environment->GetVirtualWidth();
	}

	int CogGetScreenHeight() {
		return CogEngine::GetInstance().environment->GetScreenHeight();
	}

	int CogGetVirtualHeight() {
		return CogEngine::GetInstance().environment->GetVirtualHeight();
	}

	float CogGetVirtualAspectRatio() {
		return CogEngine::GetInstance().environment->GetVirtualAspectRatio();
	}

	float CogGetScreenAspectRatio() {
		return CogEngine::GetInstance().environment->GetScreenAspectRatio();
	}

	Vec2i CogGetScreenSize() {
		return CogEngine::GetInstance().environment->GetScreenSize();
	}

	Vec2i CogGetVirtualScreenSize() {
		return CogEngine::GetInstance().environment->GetVirtualScreenSize();
	}

	void CogRunThread(ofThread* thread) {
		CogEngine::GetInstance().environment->RunThread(thread);
	}


	// =================== MSTORAGE ========================

	void CogRegisterGlobalListener(StringHash action, MsgListener* listener) {
		CogEngine::GetInstance().stage->RegisterGlobalListener(action, listener);
	}

	void CogUnregisterGlobalListener(StringHash action, MsgListener* listener) {
		CogEngine::GetInstance().stage->UnregisterGlobalListener(action, listener);
	}

	void CogSendMessage(Msg& msg, Node* actualNode) {
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg, actualNode);
	}

	void CogSendDirectMessageToListener(Msg& msg, int targetId) {
		CogEngine::GetInstance().stage->GetActualScene()->SendDirectMessageToListener(msg, targetId);
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
		CogEngine::GetInstance().logger->LogError(module, 0, format, args);
		va_end(args);
	}

	void CogLogInfo(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		CogEngine::GetInstance().logger->LogInfo(module, 0, format, args);
		va_end(args);
	}

	void CogLogTree(const char* module, int logLevel, const char* format, ...) {
		va_list args;
		va_start(args, format);
		CogEngine::GetInstance().logger->LogInfo(module, logLevel, format, args);
		va_end(args);
	}

	void CogLogDebug(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		CogEngine::GetInstance().logger->LogDebug(module, 0, format, args);
		va_end(args);
	}

	void CogLoggerFlush() {
		CogEngine::GetInstance().logger->Flush();
	}

	void CogWriteLogStage() {
		auto stage = GETCOMPONENT(Stage);
		stage->WriteInfo(0);
	}

	void CogWriteLogActualScene() {
		auto stage = GETCOMPONENT(Stage);
		auto actualScene = stage->GetActualScene();
		if (actualScene != nullptr) actualScene->WriteInfo(0);
	}

	void CogWriteTimeReport(bool restart) {
		TimeMeasure::GetInstance().Report(restart);
	}

	// =================== MRENDERER =========================
	void CogPushNodeForRendering(Node* node) {
		CogEngine::GetInstance().renderer->PushNode(node);
	}


	// =================== MCACHE =======================

	spt<ofImage> CogGet2DImage(string path) {
		return CogEngine::GetInstance().resourceCache->Get2DImage(path);
	}

	spt<ofImage> CogPreload2DImage(string path) {
		return CogEngine::GetInstance().resourceCache->Preload2DImage(path);
	}

	spt<ofVboMesh> CogGetMesh(string path) {
		return CogEngine::GetInstance().resourceCache->GetMesh(path);
	}

	spt<ofTrueTypeFont> CogGetFont(string path, int size) {
		return CogEngine::GetInstance().resourceCache->GetFont(path, size);
	}

	spt<Sound> CogGetSound(string path) {
		return CogEngine::GetInstance().resourceCache->GetSound(path);
	}

	spt<ofxXmlSettings> CogPreloadXMLFile(string path) {
		return CogEngine::GetInstance().resourceCache->PreloadXMLFile(path);
	}

	spt<ofxXmlSettings> CogLoadXMLFile(string path) {
		return CogEngine::GetInstance().resourceCache->LoadXMLFile(path);
	}

	spt<Anim> CogGetAnimation(string name) {
		return CogEngine::GetInstance().resourceCache->GetAnimation(name);
	}

	void CogStoreAnimation(spt<Anim> anim) {
		CogEngine::GetInstance().resourceCache->StoreAnimation(anim);
	}


	// ================== MCOMPONENTSTORAGE ====================

	void CogRegisterComponent(Component* value) {
		CogEngine::GetInstance().entityStorage->RegisterComponent(value);
	}

	bool CogRemoveComponent(StringHash key) {
		return CogEngine::GetInstance().entityStorage->RemoveComponent(key);
	}

	bool CogExistsComponent(StringHash key) {
		return CogEngine::GetInstance().entityStorage->ExistsComponent(key);
	}

}// namespace