#include <WinSock2.h>
#include <iostream>
#include "App.hpp"
#include "Client.hpp"
#include "PacketListener.hpp"
#include "MessageHandler.hpp"
#include "Channel.hpp"

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

        //Create our default channel.
        Channel* defaultChannel = new Channel();
        defaultChannel->SetChannelName("default");
        defaultChannel->SetUserCount(0);
        defaultChannel->SetVisible(true);
        defaultChannel->SetKeepAlive(true);

        mChannels.push_back(defaultChannel);

        return true;
    }

    void App::Run()
    {
        //The client used for connecting users. A new one is created after a connection is established.
        Client* client = new Client();
        client->SetId(0);
        client->SetUsername(std::string("User" + std::to_string(0)));
        client->SetCurrentChannel(mChannels[0]);

        for (;;)
        {
            //Check to see if there are any new connections
            if (mClients.size() < maxClients)
            {
                sockaddr* sockAddrClient = client->mClientAddr;
                int size = sizeof(*sockAddrClient);

                if (client->mSocket == SOCKET_ERROR)
                {
                    client->mSocket = accept(mServerSocket, sockAddrClient, &size);
                }

                else if (client->mSocket == INVALID_SOCKET)
                {
                    closesocket(client->mSocket);
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
                    client->SetUsername(std::string("User" + std::to_string(mCurrentClientID)));
                    client->SetCurrentChannel(mChannels[0]);

                    mChannels[0]->SetUserCount(mChannels[0]->GetUserCount() + 1);
                }
            }

            //Check to see if we have received any new data.
            mPacketListener->Update();

            //Process any messages we received, and send them to other clients.
            mMessageHandler->ProcessQueue();
        }
    }

    App::~App()
    {
        //Destroy any remaining clients.
        for (size_t i = 0; i < mClients.size(); ++i)
        {
            DELETE_NULLIFY(mClients[i]);
        }

        mClients.clear();

        //Destroy any remaining channels.
        for (size_t i = 0; i < mChannels.size(); ++i)
        {
            DELETE_NULLIFY(mChannels[i]);
        }

        mChannels.clear();

        //Destroy our App references.
        DELETE_NULLIFY(mPacketListener);
        DELETE_NULLIFY(mMessageHandler);
        DELETE_NULLIFY(mHostAddr);
    }
}