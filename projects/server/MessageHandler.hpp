#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <vector>

namespace NCS
{
	class Client;
	class Message;

	class MessageHandler
	{
	public:
		void AddMessageToQueue(Message* message);
		void ProcessQueue();

	protected:
		std::vector<Message*> queuedMessages;
	};
}

#endif