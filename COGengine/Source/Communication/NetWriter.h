#pragma once

#include <cmath>
#include <algorithm>
#include "Definitions.h"

namespace Cog {

	using namespace std;

	class NetWriter {
	private:
		tBYTE* buffer;
		tBYTE* current;
		unsigned bufferLength;
		unsigned bitOffset;

	public:

		NetWriter(unsigned bytes) {
			this->buffer = new tBYTE[bytes];
			this->bufferLength = bytes *8;
			this->current = buffer;
			this->bitOffset = 0;
			*current = 0;
		}

		void WriteBit(bool value);

		void WriteByte(tBYTE value);

		void WriteWord(tWORD value);

		void WriteDWord(tDWORD value);

		void WriteFloat(float value);

		void WriteBytes(tBYTE* data, unsigned size);

		void WriteDWords(tDWORD* data, unsigned size);

		void WriteFloats(float* data, unsigned size);

		void WriteString(string str);

		tBYTE* GetBuffer() {
			return buffer;
		}

		tBYTE* CopyData(unsigned& size);

		unsigned GetBufferBites() {
			return bufferLength;
		}

		unsigned GetUsedBites() {
			return (current - buffer) * 8 + bitOffset;
		}

		void Reset() {
			this->current = buffer;
			this->bitOffset = 0;
			*current = 0;
		}

	private:
		inline bool FreeSpace(unsigned bits) {
			return (bufferLength - GetUsedBites()) >= bits;
		}

		inline void IncrementCurrent() {
			current++;
			*current = 0;
		}
	};

} // namespace