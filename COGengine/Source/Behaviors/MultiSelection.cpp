
#include "MultiSelection.h"
#include "CogEngine.h"

namespace Cog {


	MultiSelection::MultiSelection(Setting& setting) {
		string group = setting.GetItemVal("selection_group");
		if (group.empty()) throw IllegalArgumentException("Error while loading MultiSelection behavior: expected parameter selection_group");

		this->selectionGroup = StringHash(group);

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
		RegisterListening(ACT_OBJECT_HIT_ENDED);
	}

	void MultiSelection::OnStart() {
		if (selectedImg && owner->HasState(selectedState)) {
			owner->GetShape<spt<Image>>()->SetImage(selectedImg);
		}
		else if (defaultImg) {
			owner->GetShape<spt<Image>>()->SetImage(defaultImg);
		}
		owner->SetGroup(selectionGroup);
	}

	void MultiSelection::OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_OBJECT_HIT_ENDED) && msg.GetSourceObject()->IsInGroup(selectionGroup)) {
			// check if the object was clicked (user could click on different area and release touch event over the button)
			InputEvent* evt = static_cast<InputEvent*>(msg.GetData());
			if (evt->input->handlerNodeId == msg.GetSourceObject()->GetId()) {

				if (msg.GetSourceObject()->GetId() == owner->GetId()) {
					// selected actual node
					if (!owner->HasState(selectedState)) {
						owner->SetState(selectedState);
						SendMessageToListeners(ACT_OBJECT_SELECTED, 0, nullptr, owner);
						CheckState();
					}
				}
				else {
					if (owner->HasState(selectedState)) {
						owner->ResetState(selectedState);
						CheckState();
					}

				}
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