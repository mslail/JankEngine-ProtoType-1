#pragma once

#include "Platform/WIN_32/IApplication.h"

class Application: public IApplication
{
public:
	Application();
	~Application();

private:
	VOID Initialize() override;
	VOID Update() override;
};
