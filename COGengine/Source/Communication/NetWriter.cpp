#include "NetWriter.h"
#include "Facade.h"

namespace Cog {

	void NetWriter::WriteBit(BYTE value) {
		COGASSERT(FreeSpace(1), "NetReader", "Buffer length exceeded");

		*current &= (0xFF << (8 - bitOffset));
		*current |= ((1 & value) << (7-bitOffset));
		bitOffset++;

		if (bitOffset >= 8) {
			IncrementCurrent();
			bitOffset = 0;
		}
	}

	void NetWriter::WriteByte(BYTE value) {
		COGASSERT(FreeSpace(8), "NetReader", "Buffer length exceeded");

		if (bitOffset <= 0) {
			// no offset
			*(current) = value;
			IncrementCurrent();
		}
		else {
			// first part | second part
			*current = (*current & (0xFF << (8-bitOffset))) | (value >> bitOffset);
			IncrementCurrent();
			*current = value << (8-bitOffset);
		}
	}

	void NetWriter::WriteDWord(DWORD value) {
		COGASSERT(FreeSpace(32), "NetReader", "Buffer length exceeded");
		WriteByte(value >> 24);
		WriteByte(value >> 16);
		WriteByte(value >> 8);
		WriteByte(value);
	}

	void NetWriter::WriteData(BYTE* data, unsigned size) {
		COGASSERT(FreeSpace(size * 8), "NetReader", "Buffer length exceeded");

		if (bitOffset <= 0) {
			// no offset
			memcpy(current, data, size);
			current += size;
			*current = 0;
		}
		else {
			// align each byte...
			for (unsigned i = 0; i < size; i++) {
				WriteByte(data[i]);
			}
		}
	}

	BYTE* NetWriter::CopyData(unsigned& size) {
		size = current - buffer;
		if (bitOffset > 0) size++;
		BYTE* data = new BYTE[size];
		memcpy(data, buffer, size);
		return data;
	}

} // namespace