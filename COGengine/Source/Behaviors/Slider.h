#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {


	class Slider : public Behavior {
		OBJECT_PROTOTYPE_INIT(Slider)
	protected:
		Node* body = nullptr;
		Node* text = nullptr;
		string bodyNodeTag;
		string textNodeTag;
		bool msgLocked = false;
	public:

		Slider(Node* body, Node* text) :body(body), text(text) {

		}

		Slider(Setting& setting) {
			this->bodyNodeTag = setting.GetItemVal("body");
			this->textNodeTag = setting.GetItemVal("text");

			if (this->bodyNodeTag.empty()) throw IllegalArgumentException("Error while loading Slider behavior; expected 'body' attribute in settings");
		}
		
		void OnInit();

		void OnStart();

		void SetValue(int percentage);

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace