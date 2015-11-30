#pragma once

#include <queue>

namespace NCC
{
    class PacketSender
    {
    public:
        std::queue<std::string> mQueuedPackets;

        //Processes our queued packets.
        void ProcessQueue();

        //Adds a packet to the queue.
        void AddPacketData(char* data, size_t length);

        //Sends a packet in a blocking call/immediatly.
        void SendImmediate(char* data, size_t length);
    };
}