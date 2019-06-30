#ifndef dxConsoleRender_included
#define dxConsoleRender_included
#pragma once

#include "..\..\Include\xrRender\ConsoleRender.h"

class dxConsoleRender : public IConsoleRender
{
public:
	virtual void Copy(IConsoleRender &_in);
	virtual void OnRender(bool bGame);
};

#endif	//	ConsoleRender_included