#pragma once

#include "ofxCogCommon.h"
#include "Msg.h"
#include "InputAct.h"
#include "SoundFile.h"
#include "Anim.h"

namespace Cog {

	class Behavior;
	class Component;
	class MsgListener;
	class EntityStorage;
	class Scene;

	EntityStorage* CogGetEntityStorage();

	/*************************************************************************/
	/* Facade methods that are used for centralized access to all components */
	/*************************************************************************/

	// =================== MENVIRONMENT ====================
	void CogAddSound(spt<Sound> sound);
	void CogPlaySound(spt<Sound> sound);
	vector<InputAct*>& CogGetPressedKeys();
	vector<InputAct*>& CogGetPressedPoints();
	vector<spt<Sound>>& CogGetPlayedSounds();
	int CogGetScreenRealWidth();
	int CogGetScreenRealHeight();
	int CogGetScreenWidth();
	int CogGetScreenHeight();
	float CogGetScreenRealAspectRatio();
	float CogGetScreenAspectRatio();
	void CogSetScreenAspectRatio(float ratio);
	ofVec2f CogGetScreenSize();
	void CogRunThread(ofThread* thread);

	
	// =================== MNODESTORAGE ========================
	void CogRegisterGlobalListener(StringHash action, MsgListener* listener);
	void CogUnregisterGlobalListener(StringHash action, MsgListener* listener);
	void CogSendMessage(Msg& msg, Node* actualNode);
	void CogSendDirectMessageToListener(Msg& msg, int targetId);
	void CogSendDirectMessage(StringHash action, int subaction, MsgEvent* data, Node* source, int listenerId);
	void CogSendDirectMessageToListener(StringHash action, int subaction, MsgEvent* data, Node* source, int targetId, int listenerId);

	// =================== MLOGGER =========================

	void CogLogError(const char* module, const char* format, ...);
	void CogLogInfo(const char* module, const char* format, ...);
	void CogLogDebug(const char* module, const char* format, ...);
	void CogLoggerFlush();

	// =================== MRENDERER =========================
	void CogPushNodeForRendering(Node* node);
	void CogRender();

	// =================== MCACHE =======================

	spt<ofImage> CogGet2DImage(string path);
	spt<ofImage> CogPreload2DImage(string path);
	spt<ofVboMesh> CogGetMesh(string path);
	spt<ofTrueTypeFont> CogGetFont(string path, int size);
	spt<Sound> CogGetSound(string path);
	spt<ofxXmlSettings> CogPreloadXMLFile(string path);
	spt<ofxXmlSettings> CogLoadXMLFile(string path);
	spt<Anim> CogGetAnimation(string name);
	void CogStoreAnimation(spt<Anim> anim);

	// ================== MCOMPONENTSTORAGE ====================

	void CogRegisterComponent(Component* value);

	bool CogRemoveComponent(StringHash key);

	bool CogExistsComponent(StringHash key);


}// namespace