#pragma once

#include <string>
#include "Macros.hpp"

namespace NCC
{
    class TextInputManager
    {
    public:
        TextInputManager();
        void Update();
    protected:
        void SendChatMessage();

        //Input convars
        const unsigned short mMaxMessageLength = 127;

        //Input stuff
        RO_DATA_PROPERTY(bool, AllowInput);
        RO_DATA_PROPERTY(sf::Keyboard::Key, LastKey);
        RO_DATA_PROPERTY(std::string, InputBuffer);
    };
}
