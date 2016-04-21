#pragma once

#include "Definitions.h"
#include <map>
#include "NetMessage.h"
#include "DeltaInfo.h"

using namespace std;

namespace Cog {

	class NetReader;
	class NetWriter;
	
	/**
	* Network message consisting of state of continous attributes
	*/
	class DeltaMessage : public NetData {
	private:
		// delta attributes
		map<int, float> deltas;
		// teleports (discrete jumps)
		map<int, float> teleports;
	public:
		

		DeltaMessage() {

		}

		DeltaMessage(spt<DeltaInfo> info) {
			this->deltas = info->GetDeltas();
			this->teleports = info->GetTeleports();
		}

		/**
		* Gets delta attributes mapped by their keys
		*/
		map<int, float>& GetDeltas() {
			return deltas;
		}

		/**
		* Gets teleports attributes (discrete jumps),
		* mapped by their keys
		*/
		map<int, float>& GetTeleports() {
			return teleports;
		}

		void LoadFromStream(NetReader* reader);

		void SaveToStream(NetWriter* writer);

		int GetDataLength() {
			return (deltas.size()+teleports.size()) * 8 + 4 + 4;
		}
	};

} // namespace