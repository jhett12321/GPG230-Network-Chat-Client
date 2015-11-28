#include <WinSock2.h>
#include <MSWSock.h>
#include <iostream>
#include "App.hpp"
#include "PacketListener.hpp"
#include "MessageHandler.hpp"

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
		mPacketListener = new PacketListener();
		mMessageHandler = new MessageHandler();

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
		//The client used for connecting users. A new one is created after a connection is established.
		Client* client = new Client();
		client->SetId(0);

		for (;;)
		{
			//Check to see if there are any new connections
			if (mCurrentClientID < maxClients - 1)
			{
				sockaddr* sockAddrClient = client->mClientAddr;
				int size = sizeof(*sockAddrClient);

				if (client->mSocket == SOCKET_ERROR)
				{
					client->mSocket = accept(mServerSocket, sockAddrClient, &size);
				}

				else if (client->mSocket == INVALID_SOCKET)
				{
					WSACleanup();
				}

				else
				{
					std::cout << "Client " << client->GetId() << " connected!" << std::endl;

					//Push this client into our list.
					mClients.push_back(client);

					//Increment our client ID.
					++mCurrentClientID;

					//Create a new client for the next connection.
					client = new Client();
					client->SetId(mCurrentClientID);
				}
			}

			//Check to see if we have received any new data.
			mPacketListener->Update();

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