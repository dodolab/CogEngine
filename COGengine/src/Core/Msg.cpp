#include "Msg.h"

namespace Cog {

	int Msg::idCounter = 0;

	Msg::Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType,
		int recipientId, TunnelingMode tunnelingMode, Node* contextNode, spt<MsgPayload> data)
		: id(idCounter++), action(action), senderType(senderType), senderId(senderId), recipientType(recipientType),
		recipientId(recipientId), tunnelingMode(tunnelingMode), contextNode(contextNode),
		data(data)
	{

	}

	Msg::Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType,
		TunnelingMode tunnelingMode, Node* contextNode, spt<MsgPayload> data)
		: id(idCounter++), action(action), senderType(senderType), senderId(senderId), recipientType(recipientType), 
		tunnelingMode(tunnelingMode), contextNode(contextNode), data(data)
	{

	}

	Msg::Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType,
		TunnelingMode tunnelingMode, bool sendToWholeTree, Node* contextNode, spt<MsgPayload> data)
		: id(idCounter++), action(action), senderType(senderType), senderId(senderId), recipientType(recipientType),
		tunnelingMode(tunnelingMode), sendToWholeTree(sendToWholeTree), contextNode(contextNode), data(data)
	{

	}

	Msg::Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType, Node* contextNode, spt<MsgPayload> data)
		: id(idCounter++), action(action), senderType(senderType), senderId(senderId), recipientType(recipientType),
		contextNode(contextNode), data(data)
	{

	}
}// namespace