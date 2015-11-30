#ifndef PACKET_H
#define PACKET_H

#pragma pack(push,1)
class Packet
{
public:
    enum
    {
        MESSAGE = 1,		// (Server/Client) Base Message Packet. Used for general chat and commands.
        CLIENT_INFO,		// (Client -> Server) Client Announce Packet. Server responds with it's announce packet when successful.
        SERVER_INFO		// (Server -> Client) Server Announce Packet. Indicates connection was successful.
    };

    int type;
};

class PacketMessage : public Packet
{
public:
    unsigned short length;
    char message[255];
};

class PacketClientInfo : public Packet
{
public:
    unsigned short usernameLength;
    char username[255];
};

class PacketServerInfo : public Packet
{
};

#pragma pack(pop)
#endif