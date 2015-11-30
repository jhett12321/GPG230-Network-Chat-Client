#pragma once

namespace NCS
{
    class Channel;

    class Command
    {
    public:
        enum CommandType
        {
            UNKNOWN,
            CHANNEL_JOIN,
            TELL
        };

        CommandType mType;
    };

    class CommandChannelJoin : public Command
    {
    public:
        Channel* mChannel;
    };

    class CommandTell : public Command
    {
    public:
        Client* mRecipient;
    };
}