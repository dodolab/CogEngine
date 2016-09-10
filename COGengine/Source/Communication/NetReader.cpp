#include "NetReader.h"
#include "Facade.h"

namespace Cog {

	void NetReader::ReadBit(BYTE& value) {
		COGASSERT(FreeSpace(1), "NetReader", "Buffer length exceeded");

		value = *current & (1 << (7-bitOffset));
		if (value != 0) value = 1;

		bitOffset++;

		if (bitOffset >= 8) {
			current++;
			bitOffset = 0;
		}
	}

	void NetReader::ReadByte(BYTE& value) {
		COGASSERT(FreeSpace(8), "NetReader", "Buffer length exceeded");

		if (bitOffset <= 0) {
			// no offset
			value = *current;
			current++;
		}
		else {
			// first part | second part
			value = (*current << bitOffset) | (*(current + 1) >> (8 - bitOffset));
			current++;
		}

	}

	void NetReader::ReadDWord(DWORD& value) {
		COGASSERT(FreeSpace(32), "NetReader", "Buffer length exceeded");
		value |= ReadByte() << 24;
		value |= ReadByte() << 16;
		value |= ReadByte() << 8;
		value |= ReadByte();
	}

	void NetReader::ReadData(BYTE* data, unsigned size) {
		COGASSERT(FreeSpace(size*8), "NetReader", "Buffer length exceeded");

		if (bitOffset <= 0) {
			// no offset
			memcpy(data, current, size);
			current += size;
		}
		else {
			// align each byte...
			for (unsigned i = 0; i < size; i++) {
				ReadByte(data[i]);
			}
		}
	}

} // namespace