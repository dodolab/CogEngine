#include "NetWriter.h"
#include "Facade.h"

namespace Cog {

	void NetWriter::WriteBit(bool value) {
		COGASSERT(FreeSpace(1), "NetReader", "Buffer length exceeded");

		*current &= (0xFF << (8 - bitOffset));
		*current |= ((value ? 1 : 0) << (7-bitOffset));
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

	void NetWriter::WriteFloat(float value) {
		COGASSERT(FreeSpace(32), "NetReader", "Buffer length exceeded");
		
		DWORD ivalue = *((DWORD*)&value);
		WriteDWord(ivalue);
	}

	void NetWriter::WriteBytes(BYTE* data, unsigned size) {
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

	void NetWriter::WriteDWords(DWORD* data, unsigned size) {
		COGASSERT(FreeSpace(size * 32), "NetReader", "Buffer length exceeded");

		for (unsigned i = 0; i < size; i++) {
			WriteDWord(data[i]);
		}
	}

	void NetWriter::WriteFloats(float* data, unsigned size) {
		COGASSERT(FreeSpace(size * 32), "NetReader", "Buffer length exceeded");

		for (unsigned i = 0; i < size; i++) {
			WriteFloat(data[i]);
		}
	}

	void NetWriter::WriteString(string str) {
		COGASSERT(FreeSpace(str.size() * 8 + 32), "NetReader", "Buffer length exceeded");
		COGASSERT(str.size() < 100000, "NetReader", "String limit exceeded");

		// write info about size separately
		WriteDWord(str.size());
		WriteBytes((BYTE*)str.c_str(),str.size());
	}

	BYTE* NetWriter::CopyData(unsigned& size) {
		size = current - buffer;
		if (bitOffset > 0) size++;
		BYTE* data = new BYTE[size];
		memcpy(data, buffer, size);
		return data;
	}

} // namespace