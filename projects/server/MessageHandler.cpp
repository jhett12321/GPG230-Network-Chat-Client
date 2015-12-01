#include <iostream>
#include "MessageHandler.hpp"
#include "Message.hpp"
#include "CommandHandler.hpp"
#include "Command.hpp"
#include "Client.hpp"
#include "App.hpp"
#include "Channel.hpp"

namespace NCS
{
    void MessageHandler::AddMessageToQueue(Message* message)
    {
        mQueuedMessages.push_back(message);
    }

    void MessageHandler::ProcessQueue()
    {
        for(Message* message : mQueuedMessages)
        {
            //If this message starts with a forward slash, this is a command.
            if(message->GetRawMessage().at(0) == *"/")
            {
                Command command = CommandHandler::ParseCommandMessage(message);

                //TODO actually implement this
                switch(command.mType)
                {
                    case Command::CHANNEL_JOIN:
                    {
                        break;
                    }

                    case Command::TELL:
                    {
                        break;
                    }

                    case Command::UNKNOWN:
                    {
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }

            //This is a normal message. Send the message to all other clients in the same channel.
            else
            {
                Client* sender = message->GetSender();
                Channel* currentChannel = sender->GetCurrentChannel();

                //Format our message.
                FormatChatMessage(message);

                std::cout << message->GetFormattedMessage() << std::endl;

                for (Client* client : App::Instance().mClients)
                {
                    if(client->GetCurrentChannel() == currentChannel && client != sender)
                    {
                        client->SendChatMessage(message);
                    }
                }
            }
        }

        //We have finished sending our message queue. Delete them.
        for (int i = 0; i < mQueuedMessages.size(); ++i)
        {
            DELETE_NULLIFY(mQueuedMessages[i]);
        }

        mQueuedMessages.clear();
    }

    void MessageHandler::FormatChatMessage(Message* message)
    {
        std::string channelName = message->GetSender()->GetCurrentChannel()->GetChannelName();
        std::string userName = message->GetSender()->GetUsername();

        message->SetFormattedMessage(std::string("[" + channelName + "] " + userName + ": " + message->GetRawMessage()));
    }
}
