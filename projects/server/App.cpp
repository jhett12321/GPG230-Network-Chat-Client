#include "App.hpp"

namespace NCS
{
	App& App::Instance()
	{
		static App app;
		return app;
	}

	bool App::Init()
	{
		return false;
	}

	void App::Run()
	{

	}

	App::~App()
	{

	}
}