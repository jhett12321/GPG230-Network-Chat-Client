#include "App.hpp"

int main()
{
    if (!NCC::App::Instance().Init())
    {
        return 1;
    }

    NCC::App::Instance().Run();

    return 0;
}