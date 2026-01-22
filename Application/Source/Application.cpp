#include "pch.h"


#include "Application.h"
#include "Platform/WIN_32/WinEntry.h"

ENTRYAPP(Application);

Application::Application()
{
}

Application::~Application()
{
}

VOID Application::Initialize()
{
	return VOID();
}

VOID Application::Update()
{
	MessageBox(0, L" YEETING", 0, 0);
}
