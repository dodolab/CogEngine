#pragma once

#include "Definitions.h"
#include "InputAct.h"
#include "Component.h"

namespace Cog {

	class Node;

	/**
	* Global input handler
	* When a new touch event is registered by any of HitEvent behaviors, a new request is made,
	* consisting of the node of which the hit was detected
	*
	* When an update is made, the InputHandler will select the node with the highest z-index that will
	* obtain ACT_OBJECT_HIT_STARTED message
	*/
	class InputHandler : public  Component {

	private:
		
		/**
		** Request for handling the input event
		*/
		class InputHandlerRequest {
		public:
			// node on which the hit was detected
			Node* owner;
			InputAct* touch;
			// identifier of possible behavior handler 
			int handlerBehId;
			// z-index of the owner node
			int ownerZIndex;

			InputHandlerRequest() {

			}

			InputHandlerRequest(Node* owner, InputAct* touch, int handlerBehId);
		};

		vector<InputHandlerRequest> requests;

	public:

		/**
		* Registers a new input request
		* @param owner owner of the hit event (object that was hit)
		* @param touch touch event 
		* @param handlerBehId id of the possible handler (or -1, if there is no such handler)
		* - if it isn't a general ACT_OBJECT_HIT_STARTED message will be sent
		*/
		void RegisterRequest(Node* owner, InputAct* touch, int handlerBehId);

		/**
		* Handles the inputs and select object with the highest z-index to which the 
		* message will be sent
		*/
		void HandleInputs();

		virtual void Update(const uint64 delta, const uint64 absolute) {
			HandleInputs();
		}
	};

}// namespace