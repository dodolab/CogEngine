#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Scene.h"

namespace Cog {

	/**x
	* Behavior for selection (similar to button but only one node can be selected)
	* This behavior should be attached to selection's parents, because it manipulates with more than one nodes
	*/
	class MultiSelection : public Behavior {
		OBJECT_PROTOTYPE_INIT(MultiSelection)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		ofColor selectedColor;
		ofColor defaultColor;
		StringHash selectionGroup;
		StringHash selectedState = StringHash(STATES_SELECTED);
	public:

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
		* @param selectionGroup group in where selection is controlled
		*/
		MultiSelection(spt<ofImage> defaultImg, spt<ofImage> selectedImg, StringHash selectionGroup) :
			defaultImg(defaultImg), selectedImg(selectedImg), selectionGroup(selectionGroup) {
		}

		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(StringHash selectionGroup) : selectionGroup(selectionGroup) {
		}


		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(ofColor defaultColor, ofColor selectedColor, StringHash selectionGroup) 
			: defaultColor(defaultColor), selectedColor(selectedColor), selectionGroup(selectionGroup) {
		}

		MultiSelection(Setting& setting) {
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

		void Init() {
			RegisterListening(ACT_OBJECT_HIT_ENDED);
			
			if (selectedImg && owner->HasState(selectedState)) {
				owner->GetShape<spt<Image>>()->SetImage(selectedImg);
			}
			else if(defaultImg) {
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
			owner->SetGroup(selectionGroup);
		}

		void OnMessage(Msg& msg) {
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


		void Update(const uint64 delta, const uint64 absolute) {

		}

		void CheckState() {
			
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

	};


}// namespace