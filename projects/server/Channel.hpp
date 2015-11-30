#pragma once

#include <string>
#include "Macros.hpp"

namespace NCS
{
    class Channel
    {
        PUBLIC_DATA_PROPERTY(std::string, ChannelName);
        PUBLIC_DATA_PROPERTY(int, UserCount);
        PUBLIC_DATA_PROPERTY(bool, Visible);
        PUBLIC_DATA_PROPERTY(bool, KeepAlive);
    };
}