#pragma once

#include "NetworkManager.h"
#include "NetReader.h"
#include "NetWriter.h"
#include "NetMessage.h"
#include "UpdateMessage.h"
#include "Interpolator.h"
#include "NetworkCommunicator.h"



class DummyMessage {
public:

	DummyMessage() {

	}

	DummyMessage(int num1, int num2, int num3, bool flag)
		: num1(num1), num2(num2), num3(num3), flag(flag) {

	}

	int num1;
	int num2;
	int num3;
	bool flag;

	void LoadFromStream(NetReader* reader) {
		num1 = reader->ReadDWord();
		num2 = reader->ReadDWord();
		num3 = reader->ReadDWord();
		flag = reader->ReadBit();
	}

	void SaveToStream(NetWriter* writer) {
		writer->WriteDWord(num1);
		writer->WriteDWord(num2);
		writer->WriteDWord(num3);
		writer->WriteBit(flag);
	}
};
