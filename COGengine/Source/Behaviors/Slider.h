#pragma once

#include "Behavior.h"

namespace Cog {

	class Node;

	class Slider : public Behavior {
	protected:
		Node* body = nullptr;
		Node* text = nullptr;
		string bodyNodeTag;
		string textNodeTag;
		bool msgLocked = false;
	public:

		Slider() {

		}

		Slider(Node* body, Node* text);
		
		virtual void Load(Setting& setting);
		
		void OnInit();

		void OnStart();

		void SetValue(int percentage);

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace