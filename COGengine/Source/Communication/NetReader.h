#pragma once

#include <cmath>
#include <algorithm>
#include "Definitions.h"

namespace Cog {

	typedef unsigned char BYTE;
	typedef unsigned int DWORD;

	using namespace std;

	class NetReader {
	private:
		BYTE* buffer;
		BYTE* current;
		unsigned bufferLength;
		unsigned bitOffset;

	public:

		NetReader(unsigned bytes) {
			this->buffer = new BYTE[bytes];
			this->bufferLength = bytes*8;
			this->bitOffset = 0;
			this->current = buffer;
		}

		NetReader(BYTE* data, unsigned bytes) {
			this->buffer = data;
			this->bufferLength = bytes*8;
			this->bitOffset = 0;
			this->current = data;
		}

		~NetReader() {
			delete[] buffer;
		}

		void ReadBit(bool& value);

		inline bool ReadBit() {
			bool output = 0;
			ReadBit(output);
			return output;
		}

		void ReadByte(BYTE& value);

		inline BYTE ReadByte() {
			BYTE output = 0;
			ReadByte(output);
			return output;
		}

		void ReadDWord(DWORD& value);

		inline DWORD ReadDWord() {
			DWORD output = 0;
			ReadDWord(output);
			return output;
		}

		void ReadFloat(float& value);

		inline float ReadFloat() {
			float output = 0;
			ReadFloat(output);
			return output;
		}

		void ReadBytes(BYTE* data, unsigned size);

		BYTE* ReadBytes(unsigned size) {
			BYTE* output = new BYTE[size];
			ReadBytes(output, size);
			return output;
		}

		void ReadDWords(DWORD* data, unsigned size);

		DWORD* ReadDWords(unsigned size) {
			DWORD* output = new DWORD[size];
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

		BYTE* GetBuffer() {
			return buffer;
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