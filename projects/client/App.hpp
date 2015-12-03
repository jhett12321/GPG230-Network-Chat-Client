#pragma once

#include <WinSock2.h>
#include "Macros.hpp"

namespace sf
{
    class RenderWindow;
}

namespace NCC
{
    class PacketListener;
    class PacketSender;
    class TextInputManager;

    class App
    {
    public:
        static App& Instance();
        ~App();

        //Initializer. Returns false if we are unable to bind to a port.
        bool Init();

        void Run();
    protected:
        void WindowUpdate();

        RO_DATA_PROPERTY(SOCKET, Socket);
        RO_PTR_PROPERTY(SOCKADDR_IN, Recipient);


        RO_PTR_PROPERTY(PacketListener, PacketListener);
        RO_PTR_PROPERTY(PacketSender, PacketSender);


        RO_PTR_PROPERTY(TextInputManager, TextInputManager);


        RO_PTR_PROPERTY(sf::RenderWindow, Window);
    };
}