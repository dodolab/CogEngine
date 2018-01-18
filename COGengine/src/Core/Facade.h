#pragma once

#include "Msg.h"
#include "InputAct.h"
#include "Soundfx.h"
#include "SheetAnim.h"
#include "Definitions.h"
#include "Settings.h"
#include "pugixml.h"

using namespace pugi;

namespace Cog {

	class Behavior;
	class Component;
	class BaseComponent;
	class ComponentStorage;
	class Scene;
	class Node;
	class Msg;
	class MsgPayload;
	class InputAct;
	class SheetAnim;
	class GeneralAnim;
	class AttribAnim;
	class AsyncProcess;

	ComponentStorage* CogGetComponentStorage();

	/**********************************************************************************/
	/* Facade methods that may be used for centralized access to important components */
	/**********************************************************************************/

	// =================== ofxCogEngine ====================
	
	extern int CogGetFrameCounter();
	extern uint64 CogGetAbsoluteTime();
	extern float CogGetGameSpeed();
	extern void  CogSetGameSpeed(float gameSpeed);

	// =================== Environment ====================
	
	extern void CogAddSound(Soundfx* sound);
	extern void CogPlaySound(Soundfx* sound);
	extern vector<InputAct*>& CogGetPressedKeys();
	extern vector<InputAct*>& CogGetPressedPoints();
	extern vector<Soundfx*>& CogGetPlayedSounds();
	extern void CogStopAllSounds();
	extern bool CogIsKeyPressed(int key);
	extern Vec2i CogGetMousePosition();
	extern Vec2i CogGetMouseScroll();
	extern int CogGetScreenWidth();
	extern int CogGetVirtualWidth();
	extern int CogGetScreenHeight();
	extern int CogGetVirtualHeight();
	extern float CogGetVirtualAspectRatio();
	extern float CogGetScreenAspectRatio();
	extern Vec2i CogGetScreenSize();
	extern Vec2i CogGetVirtualScreenSize();
	extern ofVec2f CogGetNativeSize();
	extern void CogSetNativeSize(int width, int height);
	extern float CogGetNativeScale();

	extern void CogRunProcess(AsyncProcess* thread);
	
	// =================== Scene ========================
	
	extern void CogRegisterGlobalListener(StrId action, BaseComponent* listener);
	extern void CogUnregisterGlobalListener(StrId action, BaseComponent* listener);
	extern void CogSendMessage(Msg& msg);

	// =================== Logger =========================

	extern void CogLogError(const char* module, const char* format, ...);
	extern void CogLogInfo(const char* module, const char* format, ...);
	extern void CogLogTree(const char* module, int logLevel, const char* format, ...);
	extern void CogLogDebug(const char* module, const char* format, ...);
	extern void CogLoggerFlush();

	extern void CogWriteLogStage();
	extern void CogWriteLogActualScene();
	extern void CogWriteTimeReport(bool restart);

	// =================== Renderer =========================
	
	extern void CogPushNodeForRendering(Node* node);

	// =================== Resources =======================

	extern spt<ofImage> CogGet2DImage(string path);
	extern spt<ofImage> CogPreload2DImage(string path);
	extern spt<ofVboMesh> CogGetVboMesh(string path);
	extern spt<ofTrueTypeFont> CogGetFont(string path, int size);
	extern Soundfx* CogGetSound(string path);
	extern spt<xml_document> CogPreloadXMLFile(string path);
	extern spt<xml_document> CogLoadXMLFile(string path);
	extern spt<GeneralAnim> CogGetAnimation(string name);
	extern spt<SheetAnim> CogGetSheetAnimation(string name);
	extern spt<AttribAnim> CogGetAttribAnimation(string name);
		
	extern void CogStoreAnimation(spt<SheetAnim> anim);
	extern Settings& CogGetGlobalSettings();
	extern Settings& CogGetDefaultSettings();
	extern Settings& CogGetProjectSettings();


}// namespace