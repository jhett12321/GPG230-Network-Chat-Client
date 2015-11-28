#ifndef PACKETLISTENER_H
#define PACKETLISTENER_H

namespace NCS
{
	class Client;

	class PacketListener
	{
	public:
		//Returns true if we received any packets this tick.
		bool Update();
	};
}

#endif