#pragma once

#include "ofxCogMain.h"
#include "Msg.h"
#include "InputAct.h"
#include "SoundFile.h"
#include "Anim.h"

namespace Cog {

	class Behavior;


	/*************************************************************************/
	/* Facade methods that are used for centralized access to all components */
	/*************************************************************************/

	// =================== MENVIRONMENT ====================
	void CogAddSound(spt<Sound> sound);
	void CogPlaySound(spt<Sound> sound);
	vector<InputAct>& CogGetPressedKeys();
	vector<InputAct>& CogGetPressedPoints();
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

	// =================== MFACTORY ========================

	float CogTranslateSpeed(float speed);

	// =================== MREPOSITORY ========================

	void CogRegisterListener(int action, Behavior* beh);
	void CogUnregisterListener(int action, Behavior* beh);
	void CogSendMessage(Msg& msg, Node* actualNode);
	void CogSendDirectMessageToBehavior(Msg& msg, int targetId);
	Node* CogFindNodeById(int id);
	int CogGetNodesCountByTag(string tag);
	Node* CogFindNodeByTag(string tag);
	vector<Node*> CogFindNodesByTag(char* tag);
	int CogGetNodesCountBySubType(int subtype);
	Node* CogFindNodeBySubType(int subtype);
	vector<Node*> CogFindNodesBySubType(int subtype);
	bool CogAddNode(Node* node);
	void CogRemoveNode(Node* node);
	bool CogAddBehavior(Behavior* beh);
	void CogRemoveBehavior(Behavior* beh);

	// =================== MLOGGER =========================

	void CogLogError(const char* module, const char* format, ...);
	void CogLogInfo(const char* module, const char* format, ...);
	void CogLogDebug(const char* module, const char* format, ...);
	void CogLoggerFlush();

	// =================== MRENDERER =========================
	void CogPushNodeForRendering(Node* node);
	void CogRender();

	// =================== MSTORAGE =======================

	spt<ofImage> CogGet2DImage(string path);
	spt<ofImage> CogPreload2DImage(string path);
	spt<ofVboMesh> CogGetMesh(string path);
	spt<ofTrueTypeFont> CogGetFont(string path, int size);
	spt<Sound> CogGetSound(string path);
	spt<ofxXmlSettings> CogPreloadXMLFile(string path);
	spt<ofxXmlSettings> CogLoadXMLFile(string path);
	spt<Anim> CogGetAnimation(string name);
	void CogStoreAnimation(spt<Anim> anim);

}