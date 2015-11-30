#pragma once

namespace NCC
{
    class Client;

    class PacketListener
    {
    public:
        //Returns true if we received any packets this tick.
        bool Update();
    };
}