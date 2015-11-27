#include "Client.hpp"

NCS::Client::Client()
{
	mSocket = SOCKET_ERROR;
	mClientAddr = new sockaddr();
}