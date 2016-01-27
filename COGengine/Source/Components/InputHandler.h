#pragma once

#include "ofxCogCommon.h"
#include "InputAct.h"
#include "SoundFile.h"
#include "Component.h"

namespace Cog {

	/**
	** Request for handling the input event
	*/
	class InputHandlerRequest {
	public:
		Node* owner;
		InputAct* touch;
		int handlerBehId;
		int ownerZIndex;

		InputHandlerRequest() {

		}

		InputHandlerRequest(Node* owner, InputAct* touch, int handlerBehId):
			owner(owner), touch(touch), handlerBehId(handlerBehId) {
			ownerZIndex = (int)owner->GetTransform().localPos.z;
		}
	};

	/**
	* Global input handler
	* When user touches the button, we have to store all objects that were touched. After that we
	* select the one with the highest z-index and that object will be handled
	*/
	class InputHandler : public  Component {

		OBJECT(InputHandler)

	private:
		vector<InputHandlerRequest> requests;

	public:

		void RegisterRequest(Node* owner, InputAct* touch, int handlerBehId) {
			auto req = InputHandlerRequest(owner, touch, handlerBehId);
			requests.push_back(req);
		}

		void HandleInputs() {
			if (requests.size() > 0) {
				// todo: there could be more input events..
				InputHandlerRequest reqToHandle;
				reqToHandle.ownerZIndex = -100000;

				for (auto request : requests) {
					// there has to be >=, because children with the same z-index will have preference
					if (request.ownerZIndex >= reqToHandle.ownerZIndex) {
						reqToHandle = request;
					}
				}

				reqToHandle.touch->handlerNodeId = reqToHandle.owner->GetId();
				reqToHandle.owner->SetState(StringHash(STATES_HIT));

				if (reqToHandle.handlerBehId == -1) SendMessageNoBubbling(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(reqToHandle.touch), reqToHandle.owner);
				else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(reqToHandle.touch), reqToHandle.owner, reqToHandle.handlerBehId);

				requests.clear();
			}
		}
	};

}// namespace