
#include "MultiSelection.h"
#include "ofxCogEngine.h"
#include "EnumConverter.h"
#include "Node.h"

namespace Cog {


	void MultiSelection::Load(Setting& setting) {
		string group = setting.GetItemVal("selection_group");
		if (group.empty()) CogLogError("MultiSelection", "Error while loading MultiSelection behavior: expected parameter selection_group");

		this->selectionGroup = StrId(group);

		// load all attributes 
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
				this->defaultColor = EnumConverter::StrToColor(defaultColorStr);
				this->selectedColor = EnumConverter::StrToColor(selectedColorStr);
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
		if (msg.HasAction(ACT_OBJECT_HIT_ENDED) && msg.GetContextNode()->IsInGroup(selectionGroup)) {
			// check if the object has been clicked (user could hit a different area and release touch over the button)
			auto evt = msg.GetDataPtr<InputEvent>();
			if (evt->input->handlerNodeId == msg.GetContextNode()->GetId()) {
				ProcessHit(msg, false);
			}
		}
		else if (msg.HasAction(ACT_STATE_CHANGED) && msg.GetContextNode()->IsInGroup(selectionGroup)) {
			ProcessHit(msg, true); // set directly, because STATE_CHANGED event has been already invoked
		}
	}

	void MultiSelection::ProcessHit(Msg& msg, bool setDirectly) {
		if (msg.GetContextNode()->GetId() == owner->GetId()) {
			// selected actual node
			if (!owner->HasState(selectedState)) {
				if (setDirectly) owner->GetStates().SetState(selectedState);
				else owner->SetState(selectedState);
				SendMessage(ACT_OBJECT_SELECTED);
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
				owner->GetMesh<Image>()->SetImage(selectedImg);
			}
			else {
				owner->GetMesh()->SetColor(selectedColor);
			}
		}
		else if (!owner->HasState(selectedState)) {
			if (defaultImg) {
				owner->GetMesh<Image>()->SetImage(defaultImg);
			}
			else {
				owner->GetMesh()->SetColor(defaultColor);
			}
		}
	}

}// namespace