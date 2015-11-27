#include "App.hpp"
#include <WinSock2.h>
#include <MSWSock.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

namespace NCC
{
	App& App::Instance()
	{
		static App app;
		return app;
	}

	bool App::Init()
	{
		WSAData wsaData;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cout << "WSA Startup failed" << std::endl;
			WSACleanup();
			return false;
		}

		//Create the socket.
		mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (mSocket == SOCKET_ERROR)
		{
			std::cout << "Error opening socket" << std::endl;
			WSACleanup();
			return false;
		}

		//Resolve IP address from hostname
		hostent *host = gethostbyname("localhost");

		if (host == nullptr)
		{
			std::cout << "Failed to resolve hostname." << std::endl;
			WSACleanup();
			return false;
		}

		//Create our socket address.
		mRecipient = new SOCKADDR_IN();

		mRecipient->sin_family = AF_INET;
		mRecipient->sin_port = htons(1300);
		mRecipient->sin_addr.s_addr = *((unsigned long*)host->h_addr); //Deprecated?

		//Attempt socket connection.
		if (connect(mSocket, (SOCKADDR*)mRecipient, sizeof(*mRecipient) + 1) != 0)
		{
			std::cout << "Failed to establish connection with server." << std::endl;
			WSACleanup();
			return false;
		}

		//Set socket to use non-blocking mode.
		u_long iMode = 1;
		ioctlsocket(mSocket, FIONBIO, &iMode);

		return true;
	}

	void App::Run()
	{
		for (;;)
		{
			//Receive data from the server.
			char buffer[1000];

			memset(buffer, 0, 999);

			//TODO cast these to our packet formats.
			int receiveDataLength = recv(mSocket, buffer, 1000, 0);
			std::cout << buffer << std::endl;

			int result = WSAGetLastError();
			if (result != WSAEWOULDBLOCK && result != 0)
			{
				std::cout << "Winsock error code: " << result << std::endl;
				std::cout << "Client disconnected!" << std::endl;

				// Shutdown our socket
				shutdown(mSocket, SD_SEND);

				// Close our socket entirely
				closesocket(mSocket);

				break;
			}

			//TODO Need to change this.
			Sleep(1000);
		}
	}

	App::~App()
	{

	}
}