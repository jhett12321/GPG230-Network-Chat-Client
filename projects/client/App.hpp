#ifndef APP_H
#define APP_H

#include "Macros.hpp"
#include <WinSock2.h>

namespace NCC
{
	class App
	{
	public:
		static App& Instance();
		~App();

		//Initializer. Returns false if we are unable to bind to a port.
		bool Init();

		void Run();
	protected:
		//Our Socket and our resolved recipient
		RO_DATA_PROPERTY(SOCKET, Socket);
		RO_PTR_PROPERTY(SOCKADDR_IN, Recipient);
	};
}

#endif