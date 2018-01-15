#pragma once

#include <cmath>
#include <algorithm>
#include "Definitions.h"

namespace Cog {

	using namespace std;

// max size of serialized string
#define STR_MAX_SIZE 100000

	/**
	* Reader of byte array received from net
	*/
	class NetReader {
	private:
		tBYTE* buffer;
		// pointer to current byte in buffer
		tBYTE* current;
		// length of the buffer
		unsigned bufferLength;
		// offset of bit in actual byte
		unsigned bitOffset;
		// if true, data won't be destructed
		bool external;

	public:

		/**
		* Creates a new NetReader
		* @param capacity buffer capacity in bytes
		*/
		NetReader(unsigned capacity);

		/**
		* Creates a new NetReader
		* @param data data to read
		* @param capacity (size of data to read) in bytes
		*/
		NetReader(tBYTE* data, unsigned capacity);

		~NetReader() {
			if(!external) delete[] buffer;
		}

		/**
		* Reads bit into bool
		*/
		void ReadBit(bool& value);

		/**
		* Reads bit as a bool
		*/
		inline bool ReadBit() {
			bool output = 0;
			ReadBit(output);
			return output;
		}

		/**
		* Reads byte into output value
		*/
		void ReadByte(tBYTE& value);

		/**
		* Reads byte
		*/
		inline tBYTE ReadByte() {
			tBYTE output = 0;
			ReadByte(output);
			return output;
		}

		/**
		* Reads word into output value
		*/
		void ReadWord(tWORD& value);

		/**
		* Reads word
		*/
		inline tWORD ReadWord() {
			tWORD output = 0;
			ReadWord(output);
			return output;
		}

		/**
		* Reads double word into output value
		*/
		void ReadDWord(tDWORD& value);

		/**
		* Reads double word
		*/
		inline tDWORD ReadDWord() {
			tDWORD output = 0;
			ReadDWord(output);
			return output;
		}

		/**
		* Reads float into output value
		*/
		void ReadFloat(float& value);

		/**
		* Reads float
		*/
		inline float ReadFloat() {
			float output = 0;
			ReadFloat(output);
			return output;
		}

		/**
		* Reads array of bytes of a given size
		* @param data array of bytes to read
		* @param size size of array in bytes
		*/
		void ReadBytes(tBYTE* data, unsigned size);

		/**
		* Reads array of bytes and returns pointer to the first byte
		* @param size size of array in bytes
		*/
		tBYTE* ReadBytes(unsigned size) {
			tBYTE* output = new tBYTE[size];
			ReadBytes(output, size);
			return output;
		}

		/**
		* Reads array of double words of a given size
		* @param data array of double words to read
		* @param size size of array in bytes
		*/
		void ReadDWords(tDWORD* data, unsigned size);

		/**
		* Reads array of double words and returns pointer to the first one
		* @param size size of array in bytes
		*/
		tDWORD* ReadDWords(unsigned size) {
			tDWORD* output = new tDWORD[size];
			ReadDWords(output, size);
			return output;
		}

		/**
		* Reads array of floats of a given size
		* @param data array of floats to read
		* @param size size of array in bytes
		*/
		void ReadFloats(float* data, unsigned size);

		/**
		* Reads array of floats and returns pointer to the first one
		* @param size size of array in bytes
		*/
		float* ReadFloats(unsigned size) {
			float* output = new float[size];
			ReadFloats(output, size);
			return output;
		}

		/**
		* Reads string
		*/
		string ReadString();

		/**
		* Gets pointer to the buffer
		*/
		tBYTE* GetBuffer() {
			return buffer;
		}

		/**
		* Gets pointer to the actual position in buffer
		*/
		tBYTE* GetActualPointer() {
			return current;
		}

		/**
		* Gets capacity of buffer in bits
		*/
		unsigned GetBufferBites() {
			return bufferLength;
		}

		/**
		* Reset buffer position
		*/
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