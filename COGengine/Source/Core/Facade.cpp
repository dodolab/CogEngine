
#include "Facade.h"
#include "CogEngine.h"
#include "Logger.h"
#include "ResourceCache.h"
#include "TimeMeasure.h"
#include "Scene.h"
#include "Environment.h"
#include "Stage.h"
#include "Renderer.h"
#include "EntityStorage.h"
#include "AsyncProcess.h"

namespace Cog {

	EntityStorage* CogGetEntityStorage() {
		return CogEngine::GetInstance().entityStorage;
	}

	int CogGetFrameCounter() {
		return CogEngine::GetInstance().GetFrameCounter();
	}

	uint64 CogGetAbsoluteTime() {
		return CogEngine::GetInstance().GetAbsoluteTime();
	}

	// =================== Environment ====================

	void CogAddSound(Soundfx* sound) {
		CogEngine::GetInstance().environment->AddSound(sound);
	}

	void CogPlaySound(Soundfx* sound) {
		CogEngine::GetInstance().environment->PlaySound(sound);
	}

	vector<InputAct*>& CogGetPressedKeys() {
		return CogEngine::GetInstance().environment->GetPressedKeys();
	}

	vector<InputAct*>& CogGetPressedPoints() {
		return CogEngine::GetInstance().environment->GetPressedPoints();
	}

	vector<Soundfx*>& CogGetPlayedSounds() {
		return CogEngine::GetInstance().environment->GetPlayedSounds();
	}

	bool CogIsKeyPressed(int key) {
		return CogEngine::GetInstance().environment->IsKeyPressed(key);
	}

	Vec2i CogGetMousePosition() {
		return CogEngine::GetInstance().environment->GetMousePosition();
	}

	Vec2i CogGetMouseScroll() {
		return CogEngine::GetInstance().environment->GetMouseScroll();
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

	void CogRunProcess(AsyncProcess* thread) {
		CogEngine::GetInstance().environment->RunProcess(thread);
	}


	// =================== EntityStorage ========================

	void CogRegisterGlobalListener(StrId action, BaseComponent* listener) {
		CogEngine::GetInstance().stage->RegisterGlobalListener(action, listener);
	}

	void CogUnregisterGlobalListener(StrId action, BaseComponent* listener) {
		CogEngine::GetInstance().stage->UnregisterGlobalListener(action, listener);
	}

	void CogSendMessage(Msg& msg) {
		CogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}


	// =================== Logger =========================

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

	// =================== Renderer =========================
	
	void CogPushNodeForRendering(Node* node) {
		CogEngine::GetInstance().renderer->PushNode(node);
	}

	// =================== ResourceCache =======================

	spt<ofImage> CogGet2DImage(string path) {
		return CogEngine::GetInstance().resourceCache->Get2DImage(path);
	}

	spt<ofImage> CogPreload2DImage(string path) {
		return CogEngine::GetInstance().resourceCache->Preload2DImage(path);
	}

	spt<ofVboMesh> CogGetVboMesh(string path) {
		return CogEngine::GetInstance().resourceCache->GetVboMesh(path);
	}

	spt<ofTrueTypeFont> CogGetFont(string path, int size) {
		return CogEngine::GetInstance().resourceCache->GetFont(path, size);
	}

	Soundfx* CogGetSound(string path) {
		return CogEngine::GetInstance().resourceCache->GetSound(path);
	}

	spt<ofxXmlSettings> CogPreloadXMLFile(string path) {
		return CogEngine::GetInstance().resourceCache->PreloadXMLFile(path);
	}

	spt<ofxXmlSettings> CogLoadXMLFile(string path) {
		return CogEngine::GetInstance().resourceCache->LoadXMLFile(path);
	}

	spt<SheetAnim> CogGetAnimation(string name) {
		return static_pointer_cast<SheetAnim>(CogEngine::GetInstance().resourceCache->GetAnimation(name));
	}

	void CogStoreAnimation(spt<SheetAnim> anim) {
		CogEngine::GetInstance().resourceCache->StoreAnimation(anim);
	}


}// namespace