
#include "Slider.h"
#include "Events.h"
#include "CogEngine.h"
#include "Scene.h"
#include "Node.h"

namespace Cog {

	Slider::Slider(Node* body, Node* text) :body(body), text(text) {

	}

	void Slider::Load(Setting& setting) {
		this->bodyNodeTag = setting.GetItemVal("body");
		this->textNodeTag = setting.GetItemVal("text");

		if (this->bodyNodeTag.empty()) CogLogError("Slider","Error while loading Slider behavior; expected 'body' attribute in settings");
	}

	void Slider::OnInit() {
		SubscribeForMessages(ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED, ACT_OBJECT_HIT_OVER, ACT_ATTR_CHANGED);
	}

	void Slider::OnStart() {
		if (body == nullptr) body = owner->GetScene()->FindNodeByTag(bodyNodeTag);
		if (text == nullptr && !textNodeTag.empty()) text = owner->GetScene()->FindNodeByTag(textNodeTag);

		if (!body->HasAttr(ATTR_IMGBOUNDS)) {
			auto image = body->GetMesh<Image>();
			body->AddAttr(ATTR_IMGBOUNDS, ofRectangle(0,0,image->GetWidth(), image->GetHeight()));
		}

		if (!owner->HasAttr(ATTR_SLIDER_VALUE)) {
			body->AddAttr(ATTR_SLIDER_VALUE, (int)0);
			SetValue(0);
		}
	}

	void Slider::SetValue(int percentage) {
		auto bounds = body->GetAttr<ofRectangle>(ATTR_IMGBOUNDS);
		string percText = ofToString(percentage) + "%";
		text->GetMesh<Text>()->SetText(percText);

		auto image = body->GetMesh<Image>();
		bounds.width = image->GetWidth()*(percentage/100.0f);
		body->ChangeAttr(ATTR_IMGBOUNDS, bounds);
	}

	void Slider::OnMessage(Msg& msg) {
		if (!msgLocked) {
			msgLocked = true;

			if (msg.GetContextNode()->GetId() == owner->GetId()) {
				if (msg.HasAction(ACT_OBJECT_HIT_OVER)) {
					
					// set value according to the position of the mouse
					spt<InputEvent> evt = msg.GetData<InputEvent>();
					auto image = body->GetMesh<Image>();

					int width = image->GetWidth();
					int absoluteWidth = image->GetWidth()*body->GetTransform().absScale.x*body->GetTransform().scale.x;
					int absolutePosX = body->GetTransform().absPos.x;

					float ratio = ofClamp((evt->input->position.x - absolutePosX) / ((float)absoluteWidth), 0, 1);
					int percentage = ratio * 100;
					this->SetValue(percentage);
					owner->ChangeAttr(ATTR_SLIDER_VALUE, (int)percentage);
				}
				else if ((msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST))) {

				}
				else if (msg.HasAction(ACT_ATTR_CHANGED)) {
					spt<AttributeChangeEvent> changeEvt = msg.GetData<AttributeChangeEvent>();
					if (changeEvt->attribute == ATTR_SLIDER_VALUE) {
						int percentage = owner->GetAttr<int>(ATTR_SLIDER_VALUE);
						SetValue(percentage);
					}
				}
			}
			msgLocked = false;
		}
	}

}// namespace