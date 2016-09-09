#pragma once

#include "EnFlags.h"
#include "MEnums.h"
#include "GMsg.h"
#include <list>
#include "MUtils.h"
#include "MFacade.h"

class GNode;

/**
* Abstract class for all behaviors
*
*/
class GBehavior{

protected:
	// identifier incremental counter
	static int idCounter;
	// element type {VIEW, MODEL}
	const ElemType elemType;
	// identifier
	const int id;
	// behavior running state
	BehState behState;
	// owner node
	GNode* owner;
	// indicator if this behavior has ended
	bool ended;

	/**
	* Creates a new behavior
	* @param elemType element type {VIEW, MODEL}
	*/
	GBehavior(ElemType elemType);

	/**
	* Registers itself as a action listener
	* @param action1 action to listen to
	*/
	void RegisterListening(int action1);

	/**
	* Registers itself as a action listener
	* @param action1 action to listen to
	* @param action2 action to listen to
	*/
	void RegisterListening(int action1, int action2);

	/**
	* Registers itself as a action listener
	* @param action1 action to listen to
	* @param action2 action to listen to
	* @param action3 action to listen to
	*/
	void RegisterListening(int action1, int action2, int action3);

	/**
	* Registers itself as a action listener
	* @param action1 action to listen to
	* @param action2 action to listen to
	* @param action3 action to listen to
	* @param action4 action to listen to
	*/
	void RegisterListening(int action1, int action2, int action3, int action4);

	/**
	* Sends a message to any set of behaviors
	* @param bubblingType setting that indicates who should process this message and how it will bubble through scene tree
	* @param action id of action; see Actions namespace for common action ids
	* @param subaction id of subaction; see Actions namespace for common action ids
	* @param data payload
	* @param source source game object that is a part of message
	*/
	void SendMessage(BubblingType bubblingType, int action, int subaction, void* data, GNode* source) const;

	/**
	* Sends a message to any set of behaviors without tree-bubbling
	* @param action id of action; see Actions namespace for common action ids
	* @param subaction id of subaction; see Actions namespace for common action ids
	* @param data payload
	* @param source source game object that is a part of message
	*/
	void SendMessageNoBubbling(int action, int subaction, void* data, GNode* source) const;

	/**
	* Sends a message to one behavior with specific id
	* @param action id of action; see Actions namespace for common action ids
	* @param subaction id of subaction; see Actions namespace for common action ids
	* @param data payload
	* @param source source game object that is a part of message
	* @param behaviorId id of behavior that should get this message
	*/
	void SendDirectMessage(int action, int subaction, void* data, GNode* source, int behaviorId) const;

	/**
	* Sets owner to behavior
	* @param beh behavior
	* @param owner node to set as an owner
	*/
	static void SetOwner(GBehavior* beh, GNode* owner){
		beh->owner = owner;
	}

public:

	virtual ~GBehavior()
	{

	}

	/**
	* Initialization procedure;
	* Any attribute that is owned only by specific behavior should
	* be created here
	*/
	virtual void Init(){

	}

	/**
	* Gets the owner node
	*/
	const GNode* GetOwner() const{
		return owner;
	}

	/**
	* Gets element type
	* @return {VIEW, MODEL}
	*/
	const ElemType GetElemType() const{
		return elemType;
	}

	/**
	* Gets element identifier
	* @return incremental value
	*/
	const int GetId() const{
		return id;
	}

	/**
	* Gets behavior running state
	* @return running state
	*/
	const BehState GetBehState() const{
		return behState;
	}

	/**
	* Sets behavior running state
	* @param val value
	*/
	void SetBehState(BehState val){
		behState = val;
	}

	/**
	* Returns true, if the behavior has ended
	*/
	bool Ended(){
		return ended;
	}

	/**
	* Finishes the behavior
	*/
	void Finish(){
		ended = true;
		SendMessageNoBubbling(Actions::BEHAVIOR_FINISHED, this->GetId(), nullptr, owner);
	}

	/**
	* Restarts the behavior
	*/
	void Restart(){
		ended = false;
		Init();
	}

	/**
	* Handler for accepting the message
	* @param GMsg received message
	*/
	virtual void OnMessage(GMsg& msg){

	}

	/**
	* Updates behavior and owning object inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun 
	*/
	virtual void Update(const uint64 delta, const uint64 absolute) = 0;

	bool operator==(int id){
		return this->id == id;
	}

	bool operator==(const GBehavior& other){
		return id == other.id;
	}

	bool operator!=(int id){
		return this->id != id;
	}

	bool operator!=(const GBehavior& other){
		return id != other.id;
	}

	// allow to access GNode private members
	friend class GNode;
};
