
#include "MultiSelection.h"
#include "CogEngine.h"

namespace Cog {


	void MultiSelection::Load(Setting& setting) {
		string group = setting.GetItemVal("selection_group");
		if (group.empty()) throw IllegalArgumentException("Error while loading MultiSelection behavior: expected parameter selection_group");

		this->selectionGroup = StrId(group);

		string defaultImg = setting.GetItemVal("default_img");
		string selectedImg = setting.GetItemVal("selected_img");

		if (!defaultImg.empty() && !selectedImg.empty()) {
			this->defaultImg = CogGet2DImage(defaultImg);
			this->selectedImg = CogGet2DImage(selectedImg);
		}
		else {
			string defaultColorStr = setting.GetItemVal("default_color");
			string selectedColorStr = setting.GetItemVal("selected_color");

			if (!defaultColorStr.empty() && !selectedColorStr.empty()) {
				this->defaultColor = StringToColor(defaultColorStr);
				this->selectedColor = StringToColor(selectedColorStr);
			}
		}
	}

	void MultiSelection::OnInit() {
		SubscribeForMessages(ACT_OBJECT_HIT_ENDED, ACT_STATE_CHANGED);
	}

	void MultiSelection::OnStart() {
		CheckState();
		owner->SetGroup(selectionGroup);
	}

	void MultiSelection::OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_OBJECT_HIT_ENDED) && msg.GetSourceObject()->IsInGroup(selectionGroup)) {
			// check if the object was clicked (user could click on different area and release touch event over the button)
			InputEvent* evt = msg.GetData<InputEvent>();
			if (evt->input->handlerNodeId == msg.GetSourceObject()->GetId()) {

				ProcessCheckMessage(msg, false);
			}
		}
		else if (msg.HasAction(ACT_STATE_CHANGED) && msg.GetSourceObject()->IsInGroup(selectionGroup)) {
			ProcessCheckMessage(msg, true); // set directly, because STATE_CHANGED event has been already raised
		}
	}

	void MultiSelection::ProcessCheckMessage(Msg& msg, bool setDirectly) {
		if (msg.GetSourceObject()->GetId() == owner->GetId()) {
			// selected actual node
			if (!owner->HasState(selectedState)) {
				if (setDirectly) owner->GetStates().SetState(selectedState);
				else owner->SetState(selectedState);
				SendMessageToListeners(ACT_OBJECT_SELECTED, 0, nullptr, owner);
				CheckState();
			}
			else {
				CheckState();
			}
		}
		else {
			if (owner->HasState(selectedState)) {
				if (setDirectly) owner->GetStates().ResetState(selectedState);
				else owner->ResetState(selectedState);
				CheckState();
			}
		}
	}

	void MultiSelection::CheckState() {

		if (owner->HasState(selectedState)) {
			if (selectedImg) {
				owner->GetShape<spt<Image>>()->SetImage(selectedImg);
			}
			else {
				owner->GetShape()->SetColor(selectedColor);
			}
		}
		else if (!owner->HasState(selectedState)) {
			if (defaultImg) {
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
			else {
				owner->GetShape()->SetColor(defaultColor);
			}
		}
	}

}// namespace