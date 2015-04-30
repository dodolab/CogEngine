
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "Msg.h"

class GNode;

class ABehavior{

protected:
	// identifier incremental counter
	static int idCounter;
	// element type {ALL, VIEW, MODEL}
	const ElemType _elemType;
	// identifier
	const int _id;
	// behavior running state
	BehState _behState;
	// message acceptation mask
	EnFlags _msgFlags;
	// owner of this behavior
	GNode* _owner;
	// dummy response message
	static Msg _dummyMsg;

	/**
	* Creates a new behavior
	* @param elemType element type {ALL, VIEW, MODEL}
	* @param msgFlags message acceptation mask
	*/
	ABehavior(ElemType elemType, EnFlags msgFlags);

	/**
	* Sends a message, expecting response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	* @param resp response that can be initialized only once
	*/
	void SendMessage(EnFlags traverse, int action, void* data, Msg& resp) const;

	/**
	* Sends a message without response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	*/
	void SendMessageNoResp(EnFlags traverse, int action, void* data) const;

public:

	/**
	* Gets element type
	* @return {ALL, VIEW, MODEL}
	*/
	const ElemType GetElemType() const;

	/**
	* Gets element identifier
	* @return incremental value
	*/
	const int GetId() const;

	/**
	* Gets behavior running state
	* @return running state
	*/
	const BehState GetBehState() const;

	/**
	* Gets owner of this behavior
	* @return pointer to game object
	*/
	const GNode* GetOwnerNode() const;

	/**
	* Gets message flags
	* @return reference to flag object
	*/
	const EnFlags& GetMessageFlags() const;

	/**
	* Sets behavior state
	* @param val value
	*/
	void SetBehState(BehState val);

	/**
	* Receives a message
	* @param msg received message
	*/
	virtual void OnMessage(Msg& msg) const = 0;

	/**
	* Updates behavior inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	*/
	virtual void Update(const uint64 delta, const uint64 absolute) const = 0;

	// only game objects can access private members (especially the owner property)
	friend class GNode;
};


#endif