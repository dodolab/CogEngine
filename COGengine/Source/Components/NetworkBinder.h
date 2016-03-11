#pragma once

#include "Component.h"

namespace Cog {

	class NetworkBinder : public Component {
		OBJECT(NetworkBinder)

	private:
		int extrapolationTimeout = 640;
	public:
		bool gotFirstMessage = false;
		bool gotSecondMessage = false;

		float parameter1 = 0;
		float parameter2 = 0;
		float parameter3 = 0;

		int parameterTime = 0;

		float previousParameter1 = 0;
		float previousParameter2 = 0;
		float previousParameter3 = 0;
		int previousParameterTime = 0;

		float nextParameter1 = 0;
		float nextParameter2 = 0;
		float nextParameter3 = 0;
		int nextParameterTime = 0;

		float deltaSpeed = 1;

		void Init();

		void OnMessage(Msg& msg);

		void AcceptDeltaUpdate(NetMessage* msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace