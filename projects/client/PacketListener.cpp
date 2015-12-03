#include <iostream>
#include "App.hpp"
#include "PacketListener.hpp"
#include "Packet.hpp"

namespace NCC
{
    bool PacketListener::Update()
    {
        //Create a union that contains packet data for all different packet types.
        union
        {
            char data[1000];

            Packet basePacket;
            PacketMessage packetMessage;
            PacketServerInfo packetServerInfo;

        } UPacket;

        bool receivedPacket = false;

        //Check our socket to see if we have received any new packets.
        //Receive data from the server.
        memset(UPacket.data, 0, 1000);

        int receiveDataLength = recv(App::Instance().GetSocket(), UPacket.data, 1000, 0);

        //TODO Check if there are some errors that would allow the socket to still function.
        //TODO Make Functions
        int result = WSAGetLastError();
        if (result != WSAEWOULDBLOCK && result != 0)
        {
            std::cout << "Winsock error code: " << result << std::endl;
            std::cout << "Server disconnected!" << std::endl;

            return false;
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
                    std::cout << messageData.c_str() << std::endl;

                    break;
                }
                case Packet::SERVER_INFO:
                {
                    break;
                }
                default:
                {
                    std::cout << "Received unknown packet type from server. Ignoring." << std::endl;
                    std::cout << UPacket.basePacket.type << std::endl;
                }
            }

            receivedPacket = true;
        }

        return receivedPacket;
    }
}
