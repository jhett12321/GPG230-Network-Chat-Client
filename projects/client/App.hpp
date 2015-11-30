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

        //Our Socket and our resolved recipient
        RO_DATA_PROPERTY(SOCKET, Socket);
        RO_PTR_PROPERTY(SOCKADDR_IN, Recipient);

        //Packet Management
        RO_PTR_PROPERTY(PacketListener, PacketListener);
        RO_PTR_PROPERTY(PacketSender, PacketSender);

        //Input Management
        RO_PTR_PROPERTY(TextInputManager, TextInputManager);

        //The Render Window
        RO_PTR_PROPERTY(sf::RenderWindow, Window);
    };
}