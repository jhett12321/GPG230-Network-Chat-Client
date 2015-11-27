#include "App.hpp"

int main()
{
	if (!NCS::App::Instance().Init())
	{
		return 1;
	}

	NCS::App::Instance().Run();

	return 0;
}