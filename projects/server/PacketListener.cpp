#include <iostream>
#include "App.hpp"
#include "PacketListener.hpp"
#include "Packet.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "Client.hpp"

namespace NCS
{
    bool PacketListener::Update()
    {
        //Create a union that contains packet data for all different packet types.
        union
        {
            char data[1000];

            Packet basePacket;
            PacketMessage packetMessage;
            PacketClientInfo packetClientInfo;

        } UPacket;

        bool receivedPacket = false;

        //Loop through our socket receivers.
        for (size_t i = 0; i < App::Instance().mClients.size(); ++i)
        {
            Client* client = App::Instance().mClients[i];

            //Receive data from the server.
            memset(UPacket.data, 0, 999);

            int receiveDataLength = recv(client->mSocket, UPacket.data, 1000, 0);

            //TODO Check if there are some errors that would allow the socket to still function.
            int result = WSAGetLastError();
            if (result != WSAEWOULDBLOCK && result != 0)
            {
                std::cout << "Winsock error code: " << result << std::endl;
                std::cout << "Client " << client->GetId() << " disconnected!" << std::endl;

                //Delete our client.
                DELETE_NULLIFY(client);
                App::Instance().mClients[i] = App::Instance().mClients[App::Instance().mClients.size() - 1];
                App::Instance().mClients.pop_back();
            }

            //We received a packet.
            if (receiveDataLength > 0)
            {
                switch (UPacket.basePacket.type)
                {
                    //This is a standard message packet. Add it to the queue for message processing.
                    case Packet::MESSAGE:
                    {
                        std::string messageData = std::string(UPacket.packetMessage.message, (size_t)UPacket.packetMessage.length);

                        Message* message = new Message();
                        message->SetSender(client);
                        message->SetRawMessage(messageData);

                        App::Instance().GetMessageHandler()->AddMessageToQueue(message);
                        break;
                    }
                    case Packet::CLIENT_INFO:
                    {
                        client->SendServerInfoToClient();
                        break;
                    }
                    default:
                    {
                        std::cout << "Received unknown packet type from client " + client->GetId() << ". Ignoring." << std::endl;
                        return false;
                    }
                }

                receivedPacket = true;
            }
        }

        return receivedPacket;
    }
}
