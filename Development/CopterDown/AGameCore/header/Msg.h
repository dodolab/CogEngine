
#ifndef MSG_H
#define MSG_H

#include "Enums.h"
#include "EnFlags.h"


/**
* Message that is used to send information between game objects
* and their behaviors

* Can be initialized using constructor or method Initialize
* Can be initialized at most once
*/
class Msg{
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
	EnFlags _traverse;
	// who is sender
	SenderType _senderType;
	// data payload (not mandatory)
	void* _data;
	// id of game object or behavior that sends this message
	int _ownerId;

public:

	/**
	* Creates uninitialized message
	* Should be initialized using Initialize method
	*/
	Msg(){
	}

	~Msg(){
		delete _data;
	}

	/**
	* Crates a new message
	* @param cat type of message (view, model)
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param senderType type of sender (attribute, behavior, game object)
	* @param ownerId id of the object who has sent this message
	* @param data payload
	* @return false, if message has been already initialized (and it can't be initialized more than once)
	*/
	Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data);

	/**
	* Crates a new message that will be traversed from the scene root
	* @param cat type of message (view, model)
	* @param action id of action; see Actions namespace for common action ids
	* @param senderType type of sender (attribute, behavior, game object)
	* @param ownerId id of the object who has sent this message
	* @param data payload
	* @return false, if message has been already initialized (and it can't be initialized more than once)
	*/
	Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data);

	/**
	* Gets id of action; see Actions for common action ids
	*/
	const int GetAction() const;

	/**
	* Gets type of this message
	*/
	const ElemType GetCategory() const;

	/**
	* Gets traversation state
	*/
	const EnFlags& GetTraverse() const;

	/**
	* Gets type of sender object
	*/
	const SenderType GetSenderType() const;

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
	const int GetOwnerId() const;

	/**
	* Indicates, if the message has been initialized
	*/
	const bool IsInitialized() const;

	/**
	* Initializes message, if it hasn't been initialized yet
	* @param cat type of message (view, model)
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param senderType type of sender (attribute, behavior, game object)
	* @param ownerId id of the object who has sent this message
	* @param data payload
	* @return false, if message has been already initialized (and it can't be initialized more than once)
	*/
	bool Initialize(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data);
};

int Msg::idCounter = 0;

Msg::Msg(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(traverse), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

Msg::Msg(ElemType cat, int action, SenderType senderType, int ownerId, void* data) : _category(cat), _traverse(EnFlags(Traverses::SCENEROOT)), _action(action),
_senderType(senderType), _ownerId(ownerId), _data(data), _id(idCounter++) {
}

const int Msg::GetAction() const{
	return _action;
}

const ElemType Msg::GetCategory() const{
	return _category;
}

const EnFlags& Msg::GetTraverse() const{
	return _traverse;
}

const SenderType Msg::GetSenderType() const{
	return _senderType;
}

const void* Msg::GetData() const{
	return _data;
}

void* Msg::GetData(){
	return _data;
}

const int Msg::GetOwnerId() const{
	return _ownerId;
}

const bool Msg::IsInitialized() const{
	return _id == 0;
}

bool Msg::Initialize(ElemType cat, EnFlags traverse, int action, SenderType senderType, int ownerId, void* data){
	if (!IsInitialized()){
		_id == idCounter++;

		_category = cat;
		_traverse = traverse;
		_action = action;
		_senderType = senderType;
		_ownerId = ownerId;
		_data = data;

		return true;
	}
	else return false;
}


#endif
