#pragma once

#include <cmath>
#include <algorithm>
#include "Definitions.h"

namespace Cog {

	typedef unsigned char BYTE;
	typedef unsigned int DWORD;

	using namespace std;

	class NetWriter {
	private:
		BYTE* buffer;
		BYTE* current;
		unsigned bufferLength;
		unsigned bitOffset;

	public:

		NetWriter(unsigned bytes) {
		
			this->buffer = new BYTE[bytes];
			this->bufferLength = bytes *8;
			this->current = buffer;
			this->bitOffset = 0;
			*current = 0;
		}

		void WriteBit(BYTE value);

		void WriteByte(BYTE value);

		void WriteDWord(DWORD value);

		void WriteData(BYTE* data, unsigned size);

		BYTE* GetBuffer() {
			return buffer;
		}

		BYTE* CopyData(unsigned& size);

		unsigned GetBufferBites() {
			return bufferLength;
		}

		void Reset() {
			this->current = buffer;
			this->bitOffset = 0;
			*current = 0;
		}

	private:
		inline bool FreeSpace(unsigned bits) {
			return (bufferLength - (buffer-current)) * 8 - bitOffset >= bits;
		}

		inline void IncrementCurrent() {
			current++;
			*current = 0;
		}
	};

} // namespace