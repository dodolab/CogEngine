#pragma once

class CheckBehavior : public Behavior {

	void OnInit() {
	}

	void OnMessage(Msg& msg) {

	}

public:
	int updateCounter = 0;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		updateCounter++;
	}
};

class MessageBehavior : public Behavior {

	bool send;

public:
	MessageBehavior(bool send) : send(send) {

	}

	void OnInit() {
		if (!send) SubscribeForMessages(StrId("MESSAGE_TEST"));
	}

	void OnMessage(Msg& msg) {
		if (!send && msg.GetAction() == "MESSAGE_TEST") {
			acceptedMessage = true;
		}
	}

public:
	bool acceptedMessage = false;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (send) this->SendMessage("MESSAGE_TEST");
	}
};

class MaxCountBehavior : public Behavior {
public:
	int maxCount;
	virtual int GetMaxCount() {
		return maxCount;
	}

	MaxCountBehavior(int maxCount) : maxCount(maxCount) {
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {

	}
};