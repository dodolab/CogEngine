#pragma once

#include "ofxCogCommon.h"
#include "Msg.h"
#include "InputAct.h"
#include "Soundfx.h"
#include "SheetAnim.h"
#include "Definitions.h"

namespace Cog {

	class Behavior;
	class Component;
	class MsgListener;
	class EntityStorage;
	class Scene;
	class Node;
	class Msg;
	class MsgEvent;
	class InputAct;

	EntityStorage* CogGetEntityStorage();

	/*************************************************************************/
	/* Facade methods that are used for centralized access to all components */
	/*************************************************************************/

	// =================== COGENGINE ====================
	int CogGetFrameCounter();
	uint64 CogGetAbsoluteTime();

	// =================== MENVIRONMENT ====================
	void CogAddSound(Soundfx* sound);
	void CogPlaySound(Soundfx* sound);
	vector<InputAct*>& CogGetPressedKeys();
	vector<InputAct*>& CogGetPressedPoints();
	vector<Soundfx*>& CogGetPlayedSounds();
	bool CogIsKeyPressed(int key);
	Vec2i CogGetMousePosition();
	int CogGetScreenWidth();
	int CogGetVirtualWidth();
	int CogGetScreenHeight();
	int CogGetVirtualHeight();
	float CogGetVirtualAspectRatio();
	float CogGetScreenAspectRatio();
	Vec2i CogGetScreenSize();
	Vec2i CogGetVirtualScreenSize();
	void CogRunThread(ofThread* thread);

	
	// =================== MNODESTORAGE ========================
	void CogRegisterGlobalListener(StrId action, MsgListener* listener);
	void CogUnregisterGlobalListener(StrId action, MsgListener* listener);
	void CogSendMessage(Msg& msg, Node* actualNode);
	void CogSendDirectMessageToListener(Msg& msg, int targetId);
	void CogSendMessageToListeners(StrId action, int subaction, MsgEvent* data, Node* source, int listenerId);
	void CogSendDirectMessageToListener(StrId action, int subaction, MsgEvent* data, Node* source, int targetId, int listenerId);

	void CogSendDirectMessageToListener(Scene* scene, Msg& msg, int targetId);
	void CogSendMessageToListeners(Scene* scene, StrId action, int subaction, MsgEvent* data, Node* source, int listenerId);
	void CogSendDirectMessageToListener(Scene* scene, StrId action, int subaction, MsgEvent* data, Node* source, int targetId, int listenerId);
	// =================== MLOGGER =========================

	void CogLogError(const char* module, const char* format, ...);
	void CogLogInfo(const char* module, const char* format, ...);
	void CogLogTree(const char* module, int logLevel, const char* format, ...);
	void CogLogDebug(const char* module, const char* format, ...);
	void CogLoggerFlush();

	void CogWriteLogStage();
	void CogWriteLogActualScene();
	void CogWriteTimeReport(bool restart);

	// =================== MRENDERER =========================
	void CogPushNodeForRendering(Node* node);

	// =================== MCACHE =======================

	spt<ofImage> CogGet2DImage(string path);
	spt<ofImage> CogPreload2DImage(string path);
	spt<ofVboMesh> CogGetMesh(string path);
	spt<ofTrueTypeFont> CogGetFont(string path, int size);
	Soundfx* CogGetSound(string path);
	spt<ofxXmlSettings> CogPreloadXMLFile(string path);
	spt<ofxXmlSettings> CogLoadXMLFile(string path);
	spt<SheetAnim> CogGetAnimation(string name);
	void CogStoreAnimation(spt<SheetAnim> anim);


}// namespace