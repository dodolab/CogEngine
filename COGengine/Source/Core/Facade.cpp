
#include "Facade.h"
#include "ofxCogEngine.h"
#include "Logger.h"
#include "Resources.h"
#include "TimeMeasure.h"
#include "Scene.h"
#include "Environment.h"
#include "Stage.h"
#include "Renderer.h"
#include "ComponentStorage.h"
#include "AsyncProcess.h"
#include "AttribAnim.h"

namespace Cog {

	ComponentStorage* CogGetComponentStorage() {
		return ofxCogEngine::GetInstance().compStorage;
	}

	int CogGetFrameCounter() {
		return ofxCogEngine::GetInstance().GetFrameCounter();
	}

	uint64 CogGetAbsoluteTime() {
		return ofxCogEngine::GetInstance().GetAbsoluteTime();
	}

	// =================== Environment ====================

	void CogAddSound(Soundfx* sound) {
		ofxCogEngine::GetInstance().environment->AddSound(sound);
	}

	void CogPlaySound(Soundfx* sound) {
		ofxCogEngine::GetInstance().environment->PlaySound(sound);
	}

	vector<InputAct*>& CogGetPressedKeys() {
		return ofxCogEngine::GetInstance().environment->GetPressedKeys();
	}

	vector<InputAct*>& CogGetPressedPoints() {
		return ofxCogEngine::GetInstance().environment->GetPressedPoints();
	}

	vector<Soundfx*>& CogGetPlayedSounds() {
		return ofxCogEngine::GetInstance().environment->GetPlayedSounds();
	}

	void CogStopAllSounds() {
		ofxCogEngine::GetInstance().environment->StopAllSounds();
	}

	bool CogIsKeyPressed(int key) {
		return ofxCogEngine::GetInstance().environment->IsKeyPressed(key);
	}

	Vec2i CogGetMousePosition() {
		return ofxCogEngine::GetInstance().environment->GetMousePosition();
	}

	Vec2i CogGetMouseScroll() {
		return ofxCogEngine::GetInstance().environment->GetMouseScroll();
	}

	int CogGetScreenWidth() {
		return ofxCogEngine::GetInstance().environment->GetScreenWidth();
	}

	int CogGetVirtualWidth() {
		return ofxCogEngine::GetInstance().environment->GetVirtualWidth();
	}

	int CogGetScreenHeight() {
		return ofxCogEngine::GetInstance().environment->GetScreenHeight();
	}

	int CogGetVirtualHeight() {
		return ofxCogEngine::GetInstance().environment->GetVirtualHeight();
	}

	float CogGetVirtualAspectRatio() {
		return ofxCogEngine::GetInstance().environment->GetVirtualAspectRatio();
	}

	float CogGetScreenAspectRatio() {
		return ofxCogEngine::GetInstance().environment->GetScreenAspectRatio();
	}

	Vec2i CogGetScreenSize() {
		return ofxCogEngine::GetInstance().environment->GetScreenSize();
	}

	Vec2i CogGetVirtualScreenSize() {
		return ofxCogEngine::GetInstance().environment->GetVirtualScreenSize();
	}

	void CogRunProcess(AsyncProcess* thread) {
		ofxCogEngine::GetInstance().environment->RunProcess(thread);
	}


	// =================== ComponentStorage ========================

	void CogRegisterGlobalListener(StrId action, BaseComponent* listener) {
		ofxCogEngine::GetInstance().stage->RegisterGlobalListener(action, listener);
	}

	void CogUnregisterGlobalListener(StrId action, BaseComponent* listener) {
		ofxCogEngine::GetInstance().stage->UnregisterGlobalListener(action, listener);
	}

	void CogSendMessage(Msg& msg) {
		ofxCogEngine::GetInstance().stage->GetActualScene()->SendMessage(msg);
	}


	// =================== Logger =========================

	void CogLogError(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		ofxCogEngine::GetInstance().logger->LogError(module, 0, format, args);
		va_end(args);
	}

	void CogLogInfo(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		ofxCogEngine::GetInstance().logger->LogInfo(module, 0, format, args);
		va_end(args);
	}

	void CogLogTree(const char* module, int logLevel, const char* format, ...) {
		va_list args;
		va_start(args, format);
		ofxCogEngine::GetInstance().logger->LogInfo(module, logLevel, format, args);
		va_end(args);
	}

	void CogLogDebug(const char* module, const char* format, ...) {
		va_list args;
		va_start(args, format);
		ofxCogEngine::GetInstance().logger->LogDebug(module, 0, format, args);
		va_end(args);
	}

	void CogLoggerFlush() {
		ofxCogEngine::GetInstance().logger->Flush();
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
		ofxCogEngine::GetInstance().renderer->PushNode(node);
	}

	// =================== Resources =======================

	spt<ofImage> CogGet2DImage(string path) {
		return ofxCogEngine::GetInstance().resources->Get2DImage(path);
	}

	spt<ofImage> CogPreload2DImage(string path) {
		return ofxCogEngine::GetInstance().resources->Preload2DImage(path);
	}

	spt<ofVboMesh> CogGetVboMesh(string path) {
		return ofxCogEngine::GetInstance().resources->GetVboMesh(path);
	}

	spt<ofTrueTypeFont> CogGetFont(string path, int size) {
		return ofxCogEngine::GetInstance().resources->GetFont(path, size);
	}

	Soundfx* CogGetSound(string path) {
		return ofxCogEngine::GetInstance().resources->GetSound(path);
	}

	spt<xml_document> CogPreloadXMLFile(string path) {
		return ofxCogEngine::GetInstance().resources->PreloadXMLFile(path);
	}

	spt<xml_document> CogLoadXMLFile(string path) {
		return ofxCogEngine::GetInstance().resources->LoadXMLFile(path);
	}

	spt<GeneralAnim> CogGetAnimation(string name) {
		return ofxCogEngine::GetInstance().resources->GetAnimation(name);
	}

	spt<SheetAnim> CogGetSheetAnimation(string name) {
		return static_pointer_cast<SheetAnim>(CogGetAnimation(name));
	}

	spt<AttribAnim> CogGetAttribAnimation(string name) {
		return static_pointer_cast<AttribAnim>(CogGetAnimation(name));
	}

	void CogStoreAnimation(spt<SheetAnim> anim) {
		ofxCogEngine::GetInstance().resources->StoreAnimation(anim);
	}

	Settings& CogGetGlobalSettings() {
		return ofxCogEngine::GetInstance().resources->GetGlobalSettings();
	}

	Settings& CogGetDefaultSettings() {
		return ofxCogEngine::GetInstance().resources->GetGlobalSettings();
	}

	Settings& CogGetProjectSettings() {
		return ofxCogEngine::GetInstance().resources->GetProjectSettings();
	}

}// namespace