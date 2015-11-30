#pragma once

namespace NCS
{
    class Message;
    class Command;

    class CommandHandler
    {
    public:
        static Command ParseCommandMessage(Message* message);
    };
}