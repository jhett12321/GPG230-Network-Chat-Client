#include <iostream>
#include "App.hpp"
#include "PacketListener.hpp"
#include "Packet.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"

namespace NCS
{
	bool PacketListener::Update()
	{
		union
		{
			char data[1000];

			Packet basePacket;
			PacketMessage packetMessage;
			PacketClientInfo packetClientInfo;

		} UPacket;

		for (Client* client : App::Instance().mClients)
		{
			//Receive data from the server.
			memset(UPacket.data, 0, 999);

			int receiveDataLength = recv(client->mSocket, UPacket.data, 1000, 0);

			int result = WSAGetLastError();
			if (result != WSAEWOULDBLOCK && result != 0)
			{
				std::cout << "Winsock error code: " << result << std::endl;
				std::cout << "Client disconnected!" << std::endl;

				// Shutdown our socket
				shutdown(client->mSocket, SD_SEND);

				// Close our socket entirely
				closesocket(client->mSocket);

				return false;
			}

			if (receiveDataLength > 0)
			{
				switch (UPacket.basePacket.type)
				{
					case Packet::MESSAGE:
					{
						std::string messageData = std::string(UPacket.packetMessage.message, (size_t)UPacket.packetMessage.length);

						Message* message = new Message();
						message->SetSender(client);
						message->SetMessage(messageData);

						App::Instance().GetMessageHandler()->AddMessageToQueue(message);
						break;
					}
					case Packet::CLIENT_INFO:
					{
						break;
					}
					default:
					{
						std::cout << "Received unknown packet type from client " + client->GetId() << ". Ignoring." << std::endl;
						return false;
					}
				}

				return true;
			}

			return false;
		}
	}
}
