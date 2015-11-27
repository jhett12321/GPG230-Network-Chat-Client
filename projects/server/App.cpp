#include "App.hpp"
#include <WinSock2.h>
#include <MSWSock.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

namespace NCS
{
	App& App::Instance()
	{
		static App app;
		return app;
	}

	bool App::Init()
	{
		WSAData wsaData;

		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
		{
			std::cout << "WSA Startup failed! Error: " << result << std::endl;
			WSACleanup();
			return false;
		}

		//Create the socket.
		mServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (mServerSocket == SOCKET_ERROR)
		{
			result = WSAGetLastError();

			std::cout << "Error opening socket! Error: " << result << std::endl;
			WSACleanup();
			return false;
		}

		//Create our clients.
		for (int i = 0; i < maxClients; ++i)
		{
			Client* client = new Client();
			client->SetId(i);

			mClients.push_back(client);
		}

		//Create our socket address.
		mHostAddr = new SOCKADDR_IN();

		mHostAddr->sin_family = AF_INET;
		mHostAddr->sin_port = htons(1300);
		mHostAddr->sin_addr.s_addr = INADDR_ANY; //Deprecated?

		//Attempt handshake socket binding.
		if (bind(mServerSocket, (SOCKADDR*)mHostAddr, sizeof(*mHostAddr)) == SOCKET_ERROR)
		{
			result = WSAGetLastError();

			std::cout << "Failed to bind socket! Error: " << result << std::endl;
			WSACleanup();
			return false;
		}

		if (listen(mServerSocket, 1))
		{
			result = WSAGetLastError();

			std::cout << "Failed to listen! Error: " << result  << std::endl;
			WSACleanup();
			return false;
		}

		//Set handshake socket to use non-blocking mode.
		u_long iMode = 1;
		ioctlsocket(mServerSocket, FIONBIO, &iMode);

		return true;
	}

	void App::Run()
	{
		for (;;)
		{
			//Check to see if there are any new connections
			if (mCurrentClientID < maxClients)
			{
				sockaddr* sockAddrClient = mClients[mCurrentClientID]->mClientAddr;
				int size = sizeof(*sockAddrClient);

				if (mClients[mCurrentClientID]->mSocket == SOCKET_ERROR)
				{
					mClients[mCurrentClientID]->mSocket = accept(mServerSocket, sockAddrClient, &size);
				}

				else if (mClients[mCurrentClientID]->mSocket == INVALID_SOCKET)
				{
					WSACleanup();
				}

				else
				{
					++mCurrentClientID;
					std::cout << "A client connected! " << std::endl;
				}
			}

			char *testMessage = "Welcome to the server!";
			for (int i = 0; i <= mCurrentClientID; ++i)
			{
				send(mClients[i]->mSocket, testMessage, strlen(testMessage), 0);
			}

			//TODO Need to change this.
			Sleep(1000);
		}
	}

	App::~App()
	{

	}
}