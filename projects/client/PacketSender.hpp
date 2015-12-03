#pragma once

#include <queue>

namespace NCC
{
    class PacketSender
    {
    public:
        PacketSender(){}

        //Processes our queued packets.
        void ProcessQueue();

        //Adds a packet to the queue.
        void AddPacketData(char* data, size_t length);

        //Sends a packet in a blocking call/immediately.
        void SendImmediate(char* data, size_t length);
    protected:
        std::queue<std::string> mQueuedPackets;
        PacketSender(const PacketSender&) = delete;
    };
}