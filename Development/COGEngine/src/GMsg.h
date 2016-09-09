#ifndef MSG_H
#define MSG_H

#include "Enums.h"
#include "EnFlags.h"
#include <functional>

class GMsg;

typedef std::tr1::function<void(GMsg const &)> MsgCallback;

class GNode;

// traversation settings
class Traversation{

public:

	Traversation(ScopeType scopeType, bool deep, bool bubbleDown) : scope(scopeType), deep(deep), bubbleDown(bubbleDown){

	}
	// scope object
	ScopeType scope;
	// if true, all object below/up will be traversed
	bool deep;
	// if true, traversation goes up-down; otherwise it goes down-up
	bool bubbleDown;
};

/**
* Message that is used to send information between game objects
* and their behaviors

* Can be initialized using constructor or method Initialize
* Can be initialized at most once
*/
class GMsg{
private:
	// identifier counter
	static int idCounter;
	// id of this message
	int _id;
	// type of action
	int _action;
	// category
	ElemType _category;
	// for who is message intended
	Traversation _traverse;
	// data payload (not mandatory)
	void* _data;
	// id of behavior that sends this message
	int _behaviorId;
	// game object whose behavior has created this message
	GNode* _sourceObj;

	void Construct(){

	}

public:

	~GMsg(){
		//delete _data;
	}

	/**
	* Creates a new message
	* @param cat type of message (view, model)
	* @param who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param behaviorId id of the behavior who has sent this message
	* @param sourceObj source object
	* @param data payload
	* @return false, if message has been already initialized (and it can't be initialized more than once)
	*/
	GMsg(ElemType cat, Traversation traverse, int action, int behaviorId, GNode* sourceObj, void* data);


	/**
	* Gets id of action; see Actions for common action ids
	*/
	const int GetAction() const;

	/**
	* Gets type of this message
	*/
	const ElemType GetCategory() const;

	/**
	* Gets traversation configuration
	*/
	Traversation& GetTraverse();

	/**
	* Gets data payload
	*/
	const void* GetData() const;
	
	/**
	* Gets data payload
	*/
	void* GetData();

	/**
	* Gets identifier of the sender object
	*/
	const int GetBehaviorId() const;

	GNode* GetSourceObject(){
		return _sourceObj;
	}

};


#endif
