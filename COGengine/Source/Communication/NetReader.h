#pragma once

#include <cmath>
#include <algorithm>
#include "Definitions.h"

namespace Cog {

	using namespace std;

	class NetReader {
	private:
		tBYTE* buffer;
		tBYTE* current;
		unsigned bufferLength;
		unsigned bitOffset;
		bool external;

	public:

		NetReader(unsigned bytes) {
			this->buffer = new tBYTE[bytes];
			this->bufferLength = bytes*8;
			this->bitOffset = 0;
			this->current = buffer;
			this->external = false;
		}

		NetReader(tBYTE* data, unsigned bytes) {
			this->buffer = data;
			this->bufferLength = bytes*8;
			this->bitOffset = 0;
			this->current = data;
			this->external = true;
		}

		~NetReader() {
			if(!external) delete[] buffer;
		}

		void ReadBit(bool& value);

		inline bool ReadBit() {
			bool output = 0;
			ReadBit(output);
			return output;
		}

		void ReadByte(tBYTE& value);

		inline tBYTE ReadByte() {
			tBYTE output = 0;
			ReadByte(output);
			return output;
		}

		void ReadWord(tWORD& value);

		inline tWORD ReadWord() {
			tWORD output = 0;
			ReadWord(output);
			return output;
		}

		void ReadDWord(tDWORD& value);

		inline tDWORD ReadDWord() {
			tDWORD output = 0;
			ReadDWord(output);
			return output;
		}

		void ReadFloat(float& value);

		inline float ReadFloat() {
			float output = 0;
			ReadFloat(output);
			return output;
		}

		void ReadBytes(tBYTE* data, unsigned size);

		tBYTE* ReadBytes(unsigned size) {
			tBYTE* output = new tBYTE[size];
			ReadBytes(output, size);
			return output;
		}

		void ReadDWords(tDWORD* data, unsigned size);

		tDWORD* ReadDWords(unsigned size) {
			tDWORD* output = new tDWORD[size];
			ReadDWords(output, size);
			return output;
		}

		void ReadFloats(float* data, unsigned size);

		float* ReadFloats(unsigned size) {
			float* output = new float[size];
			ReadFloats(output, size);
			return output;
		}

		string ReadString();

		tBYTE* GetBuffer() {
			return buffer;
		}

		tBYTE* GetActualPointer() {
			return current;
		}

		unsigned GetBufferBites() {
			return bufferLength;
		}

		void Reset() {
			this->current = buffer;
			this->bitOffset = 0;
		}

	private:
		inline bool FreeSpace(unsigned bits) {
			return (bufferLength - (buffer-current)) * 8 - bitOffset >= bits;
		}
	};

} // namespace