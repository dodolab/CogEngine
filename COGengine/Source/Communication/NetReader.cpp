#include "NetReader.h"
#include "Facade.h"

namespace Cog {

	void NetReader::ReadBit(bool& value) {
		COGASSERT(FreeSpace(1), "NetReader", "Buffer length exceeded");

		value = (*current & (1 << (7-bitOffset))) != 0;

		bitOffset++;

		if (bitOffset >= 8) {
			current++;
			bitOffset = 0;
		}
	}

	void NetReader::ReadByte(tBYTE& value) {
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

	void NetReader::ReadWord(tWORD& value) {
		COGASSERT(FreeSpace(16), "NetReader", "Buffer length exceeded");
		value = 0;
		value |= ReadByte() << 8;
		value |= ReadByte();
	}

	void NetReader::ReadDWord(tDWORD& value) {
		COGASSERT(FreeSpace(32), "NetReader", "Buffer length exceeded");
		value = 0;
		value |= ReadByte() << 24;
		value |= ReadByte() << 16;
		value |= ReadByte() << 8;
		value |= ReadByte();
	}

	void NetReader::ReadFloat(float& value) {
		COGASSERT(FreeSpace(32), "NetReader", "Buffer length exceeded");

		tDWORD iVal = ReadDWord();
		
		value = *((float*)(&iVal));
	}

	void NetReader::ReadBytes(tBYTE* data, unsigned size) {
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

	void NetReader::ReadDWords(tDWORD* data, unsigned size) {
		for (unsigned i = 0; i < size; i++) {
			ReadDWord(data[i]);
		}
	}

	void NetReader::ReadFloats(float* data, unsigned size) {
		for (unsigned i = 0; i < size; i++) {
			ReadFloat(data[i]);
		}
	}

	string NetReader::ReadString() {
		tDWORD size = ReadDWord();
		COGASSERT(size < 100000, "NetReader", "Unexpected string size in byte array");

		tBYTE* bytes = ReadBytes(size);
		string output = string((char*)bytes, (int)size);
		delete bytes;
		return output;
	}

} // namespace