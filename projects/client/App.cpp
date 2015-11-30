#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "PacketListener.hpp"
#include "TextInputManager.hpp"
#include "PacketSender.hpp"
#include "App.hpp"

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
        mPacketListener = new PacketListener();
        mPacketSender = new PacketSender();
        mTextInputManager = new TextInputManager();

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
        char *hostname = "localhost";
        struct addrinfo hints, *res;
        struct in_addr addr;
        int err;

        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET;

        if ((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
        {
            std::cout << "Failed to resolve hostname." << std::endl;
            WSACleanup();
            return false;
        }

        addr.S_un = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un;

        freeaddrinfo(res);

        //Create our socket address.
        mRecipient = new SOCKADDR_IN();

        mRecipient->sin_family = AF_INET;
        mRecipient->sin_port = htons(1300);
        mRecipient->sin_addr = addr;

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

        //Create the SFML Window
        mWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Network Chat Client");

        //Create
        return true;
    }

    void App::Run()
    {
        while(mWindow->isOpen())
        {
            //Process our Input
            mTextInputManager->Update();

            //Send our queued packets.
            mPacketSender->ProcessQueue();

            //Receive data from the server.
            mPacketListener->Update();

            //Update our window
            WindowUpdate();

            int result = WSAGetLastError();
            if (result != WSAEWOULDBLOCK && result != 0)
            {
                std::cout << "Winsock error code: " << result << std::endl;

                // Shutdown our socket
                shutdown(mSocket, SD_SEND);

                // Close our socket entirely
                closesocket(mSocket);

                break;
            }
        }
    }

    void App::WindowUpdate()
    {
        mWindow->clear();
    }

    App::~App()
    {
        DELETE_NULLIFY(mPacketSender);
        DELETE_NULLIFY(mPacketListener);
        DELETE_NULLIFY(mTextInputManager);

        DELETE_NULLIFY(mRecipient);
    }
}