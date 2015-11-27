#include "App.hpp"

namespace NCC
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