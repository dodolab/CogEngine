#include "InputHandler.h"
#include "Node.h"

namespace Cog {

	InputHandler::InputHandlerRequest::InputHandlerRequest(Node* owner, InputAct* touch, int handlerBehId) :
		owner(owner), touch(touch), handlerBehId(handlerBehId) {
		ownerZIndex = (int)owner->GetTransform().localPos.z;
	}

	void InputHandler::RegisterRequest(Node* owner, InputAct* touch, int handlerBehId) {
		auto req = InputHandlerRequest(owner, touch, handlerBehId);
		requests.push_back(req);
	}

	void InputHandler::HandleInputs() {
		if (requests.size() > 0) {

			InputHandlerRequest reqToHandle;
			reqToHandle.ownerZIndex = -100000;

			for (auto request : requests) {
				// children with the same z-index will be preffered
				if (request.ownerZIndex >= reqToHandle.ownerZIndex) {
					reqToHandle = request;
				}
			}

			reqToHandle.touch->handlerNodeId = reqToHandle.owner->GetId();
			reqToHandle.owner->SetState(StrId(STATE_HIT));

			if (reqToHandle.handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_STARTED, spt<InputEvent>(new InputEvent(reqToHandle.touch)), reqToHandle.owner);
			else SendMessageToBehavior(ACT_OBJECT_HIT_STARTED, spt<InputEvent>(new InputEvent(reqToHandle.touch)), reqToHandle.owner, reqToHandle.handlerBehId);

			requests.clear();
		}
	}


} // namespace