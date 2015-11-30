#pragma once

#include <vector>

namespace NCS
{
    class Client;
    class Message;

    class MessageHandler
    {
    public:
        void AddMessageToQueue(Message* message);
        void ProcessQueue();

    protected:
        std::vector<Message*> mQueuedMessages;
        void FormatChatMessage(Message* message);
    };
}