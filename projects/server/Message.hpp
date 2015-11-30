#pragma once

#include <string>
#include "Macros.hpp"

namespace NCS
{
    class Client;

    class Message
    {
    protected:
        PUBLIC_DATA_PROPERTY(std::string, RawMessage);
        PUBLIC_DATA_PROPERTY(std::string, FormattedMessage);
        PUBLIC_PTR_PROPERTY(Client, Sender);
    };
}