#include <Packet.hpp>
#include "Client.hpp"
#include "Message.hpp"

namespace
{
    char buffer[1000];
}

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
        PacketMessage* messagePacket = new (&buffer[0]) PacketMessage();

        messagePacket->type = Packet::MESSAGE;

        //Set our length value so we know how big it is when getting our string back.
        messagePacket->length = chatMessage.length();

        //Copy our message data into the packet.
        //Add 1 to our size to get the null terminator.
        strcpy_s(messagePacket->message, chatMessage.size() + 1, chatMessage.c_str());

        //Send our packet to the client.
        //Since we defined our message array as a 1, we only need to add the size of the chat message.
        send(mSocket, reinterpret_cast<char*>(messagePacket), sizeof(PacketMessage) + chatMessage.size(), 0);
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
