#pragma once

#include "Component.h"
#include "DeltaInfo.h"

namespace Cog {

	class DeltaUpdate : public Component {
		OBJECT(DeltaUpdate)

	private:
		int extrapolationTimeout = 640;
	public:
		spt<DeltaInfo> previous;
		spt<DeltaInfo> actual;
		spt<DeltaInfo> next;

		float deltaSpeed = 1;

		void Init();

		void OnMessage(Msg& msg);

		void AcceptDeltaUpdate(spt<DeltaInfo> msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace