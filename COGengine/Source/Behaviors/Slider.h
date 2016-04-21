#pragma once

#include "Behavior.h"

namespace Cog {

	class Node;

	/**
	* Slider component consisting of a body and text
	* The value of the slider is set by dragging over the body of the node
	*/
	class Slider : public Behavior {
	protected:
		Node* body = nullptr;
		Node* text = nullptr;
		string bodyNodeTag;
		string textNodeTag;
		// indicator that prevents capturing message until
		// the handler of previous message has finished
		bool msgLocked = false;
	public:

		Slider() {

		}

		Slider(Node* body, Node* text);
		
		virtual void Load(Setting& setting);
		
		void OnInit();

		void OnStart();

		/**
		* Sets the slider value
		* @param percentage percentage value in rage <0,100>
		*/
		void SetValue(int percentage);

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace