#ifndef CLIENT_H
#define CLIENT_H

#include "Macros.hpp"
#include <WinSock2.h>

namespace NCS
{
	class Client
	{
	public:
		Client();
		~Client();
		void SendMessage(char*);

		//This client socket and our host address.
		SOCKET mSocket;
		sockaddr* mClientAddr;
	protected:
		//std::vector<"string"> chatChannels;

		//This Client's ID
		PUBLIC_DATA_PROPERTY(int, Id);
	};
}

#endif