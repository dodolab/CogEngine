#pragma once

#include "Definitions.h"
#include <map>
#include "NetMessage.h"
#include "UpdateInfo.h"

using namespace std;

namespace Cog {

	class NetReader;
	class NetWriter;
	
	/**
	* Network message consisting of state of continous attributes
	*/
	class UpdateMessage : public NetData {
	private:
		// continuous attributes
		map<int, float> continuousValues;
		// discrete attributes
		map<int, float> discreteValues;
	public:
		

		UpdateMessage() {

		}

		UpdateMessage(spt<UpdateInfo> info) {
			this->continuousValues = info->GetContinuousValues();
			this->discreteValues = info->GetDiscreteValues();
		}

		/**
		* Gets continuous values mapped by their keys
		*/
		map<int, float>& GetContinuousValues() {
			return continuousValues;
		}

		/**
		* Gets discrete values mapped by their keys
		*/
		map<int, float>& GetDiscreteValues() {
			return discreteValues;
		}

		void LoadFromStream(NetReader* reader);

		void SaveToStream(NetWriter* writer);

		int GetDataLength() {
			return (continuousValues.size()+discreteValues.size()) * 8 + 4 + 4;
		}
	};

} // namespace