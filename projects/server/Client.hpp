#pragma once

#include <WinSock2.h>
#include <string>
#include "Macros.hpp"

namespace NCS
{
    class Message;
    class Channel;

    class Client
    {
    public:
        Client();
        ~Client();
        void SendChatMessage(Message* message);
        void SendServerInfoToClient();

        //This client socket and our host address.
        SOCKET mSocket;
        sockaddr* mClientAddr;
    protected:
        //This Client's ID/Username.
        PUBLIC_DATA_PROPERTY(int, Id);
        PUBLIC_DATA_PROPERTY(std::string, Username);

        //This Client's Active Channel.
        PUBLIC_PTR_PROPERTY(Channel, CurrentChannel);

        //Our buffer used for holding packets to send.
        static char buffer[1000];
    };
}