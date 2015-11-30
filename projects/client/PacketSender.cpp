#include <iostream>
#include "App.hpp"
#include "PacketSender.hpp"

namespace NCC
{
    void PacketSender::AddPacketData(char* data, size_t length)
    {
        std::string packetData = std::string(data, length);

        mQueuedPackets.push(packetData);
    }

    void PacketSender::SendImmediate(char* data, size_t length)
    {
        send(App::Instance().GetSocket(), data, length, 0);
    }

    void PacketSender::ProcessQueue()
    {
        while (!mQueuedPackets.empty())
        {
            std::string data = mQueuedPackets.front();

            send(App::Instance().GetSocket(), data.c_str(), data.length(), 0);

            mQueuedPackets.pop();
        }
    }
}