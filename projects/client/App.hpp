#ifndef APP_H
#define APP_H

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
	};
}

#endif