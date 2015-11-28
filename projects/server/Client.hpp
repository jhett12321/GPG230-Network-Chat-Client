#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <string>
#include "Macros.hpp"

namespace NCS
{
	class Client
	{
	public:
		Client();
		~Client();
		void SendMessage(std::string &message);

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