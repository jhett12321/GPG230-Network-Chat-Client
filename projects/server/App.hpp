#ifndef APP_H
#define APP_H

#include <WinSock2.h>
#include <vector>
#include "Client.hpp"
#include "Macros.hpp"

namespace NCS
{
	class App
	{
	public:
		static App& Instance();
		~App();

		//Initializer. Returns false if we are unable to bind to a port.
		bool Init();

		void Run();
	protected:
		//Our Socket and our host address.
		RO_DATA_PROPERTY(SOCKET, ServerSocket);
		RO_PTR_PROPERTY(SOCKADDR_IN, HostAddr);

		//Clients
		static const int maxClients = 32;
		std::vector<NCS::Client*> mClients;

		RO_DATA_PROPERTY(int, CurrentClientID);
	};
}

#endif