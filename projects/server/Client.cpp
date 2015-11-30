#include <Packet.hpp>
#include "Client.hpp"
#include "Message.hpp"

namespace NCS
{
    Client::Client()
    {
        mSocket = SOCKET_ERROR;
        mClientAddr = new sockaddr();
    }

    void Client::SendChatMessage(Message* message)
    {
        std::string chatMessage = message->GetFormattedMessage();

        //Create our packet, and set its type.
        PacketMessage messagePacket = PacketMessage();
        messagePacket.type = Packet::MESSAGE;

        //Set our length value so we know how big it is when getting our string back.
        messagePacket.length = chatMessage.length();

        //Initialize our packet char array to copy our message.
        *messagePacket.message = *new char[chatMessage.size() + 1];

        //Null Terminator
        messagePacket.message[chatMessage.size()] = 0;

        //Copy the message into our char array.
        memcpy(messagePacket.message, chatMessage.c_str(), chatMessage.size());

        //Send our packet to the client.
        send(mSocket, reinterpret_cast<char*>(&messagePacket), sizeof(messagePacket), 0);
    }

    void Client::SendServerInfoToClient()
    {
        PacketServerInfo serverInfo;
        serverInfo.type = Packet::SERVER_INFO;

        send(mSocket, reinterpret_cast<char*>(&serverInfo), sizeof(serverInfo), 0);
    }

    Client::~Client()
    {
        delete mClientAddr;
    }

}
