#pragma once

#include "Behavior.h"

namespace Cog {

	class Node;

	class Slider : public Behavior {
		OBJECT_PROTOTYPE_INIT(Slider)
	protected:
		Node* body = nullptr;
		Node* text = nullptr;
		string bodyNodeTag;
		string textNodeTag;
		bool msgLocked = false;
	public:

		Slider(Node* body, Node* text);
		
		Slider(Setting& setting);
		
		void OnInit();

		void OnStart();

		void SetValue(int percentage);

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace