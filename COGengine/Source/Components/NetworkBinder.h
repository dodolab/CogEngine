#pragma once

#include "Component.h"

namespace Cog {

	class NetworkBinder : public Component {
		OBJECT(NetworkBinder)

	private:

	public:
		bool gotFirstMessage = false;
		bool gotSecondMessage = false;

		float parameter = 0;
		int parameterTime = 0;

		float previousParameter = 0;
		int previousParameterTime = 0;

		float nextParameter = 0;
		int nextParameterTime = 0;

		float deltaSpeed = 1;

		void Init();

		void OnMessage(Msg& msg);

		void AcceptDeltaUpdate(NetMessage* msg);

		virtual void Update(const uint64 delta, const uint64 absolute);

	private:

	};

}// namespace