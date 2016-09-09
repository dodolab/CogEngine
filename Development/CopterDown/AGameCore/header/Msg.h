
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
	int _id = 0;
	// type of action
	int _action = 0;
	// category
	ElemType _category;
	// for who is message intended
	EnFlags _traverse;
	// who is sender
	SenderType _senderType;
	// data payload (not mandatory)
	void* _data;
	// id of game object or behavior that sends this message
	int _ownerId = 0;

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
	* Creates a new message
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
	* Creates a new message that will be traversed from the scene root
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
	EnFlags& GetTraverse();

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


#endif
