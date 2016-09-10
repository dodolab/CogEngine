#pragma once

#include "Component.h"
#include "DeltaInfo.h"

namespace Cog {

	class DeltaUpdate : public Component {
	private:
		int messagesObtained = 0;
		uint64 initTime = 0;
	public:
		spt<DeltaInfo> previous;
		spt<DeltaInfo> actual;
		spt<DeltaInfo> next;

		float deltaSpeed = 1;
		
		void OnInit();

		void OnMessage(Msg& msg);

		void AcceptDeltaUpdate(spt<DeltaInfo> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace