#pragma once

#include "Definitions.h"
#include <map>


namespace Cog {

	/**
	* Entity describing delta informations about continuous attributes
	* Is used during network synchronization
	*/
	class DeltaInfo {
	private:
		// time the state was captured
		uint64 time;
		// delta attributes
		map<int, float> deltas;
		// teleports (discrete jumps)
		map<int, float> teleports;
	public:

		DeltaInfo() {

		}

		DeltaInfo(uint64 time, map<int,float>& deltas, map<int,float>& teleports) 
			: time(time), deltas(deltas), teleports(teleports){

		}

		DeltaInfo(uint64 time, map<int, float>& deltas)
			: time(time), deltas(deltas) {

		}

		/**
		* Gets time the state was captured
		*/
		uint64 GetTime() const {
			return time;
		}

		/**
		* Sets time the state was captured
		*/
		void SetTime(uint64 time) {
			this->time = time;
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

		/**
		* Gets delta value by key
		*/
		float GetDelta(int key) {
			auto found = deltas.find(key);
			return (found != deltas.end()) ? (*found).second : 0;
		}
		
		/**
		* Gets teleport value by its key
		*/
		float GetTeleport(int key) {
			auto found = teleports.find(key);
			return (found != teleports.end()) ? (*found).second : 0;
		}

		/**
		* Gets either delta or teleport by its key
		*/
		float GetVal(int key) {
			auto foundDelta = deltas.find(key);

			if (foundDelta != deltas.end()) return (*foundDelta).second;

			auto foundTelep = teleports.find(key);
			return (foundTelep != teleports.end()) ? (*foundTelep).second : 0;
		}

		friend class DeltaUpdate;
	};


} // namespace