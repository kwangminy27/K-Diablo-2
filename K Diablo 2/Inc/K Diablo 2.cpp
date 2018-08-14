#include "stdafx.h"
#include "core.h"
#include "resource.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR command_line, int command_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto const& core = Core::GetSingleton();

	if (!core->Initialize(L"K Diablo 2", L"K Diablo 2", instance, LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON1))))
		return -1;

	core->Run();

	return 0;
}