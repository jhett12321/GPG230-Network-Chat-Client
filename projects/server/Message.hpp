#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "Macros.hpp"

namespace NCS
{
	class Client;

	class Message
	{
	protected:
		PUBLIC_DATA_PROPERTY(std::string, Message);
		PUBLIC_PTR_PROPERTY(Client, Sender);
	};
}

#endif