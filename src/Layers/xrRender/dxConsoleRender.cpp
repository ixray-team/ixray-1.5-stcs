#include "stdafx.h"
#include "dxConsoleRender.h"

void dxConsoleRender::Copy(IConsoleRender &_in)
{
	*this = * (dxConsoleRender*)&_in;
}

void dxConsoleRender::OnRender(bool bGame)
{
	VERIFY	(HW.pDevice);


	D3DRECT R = { 0,0,Device.dwWidth,Device.dwHeight};
	if		(bGame) R.y2 /= 2;

#ifdef	USE_DX10
	//	TODO: DX10: Implement console background clearing for DX10
#else	//	USE_DX10
	CHK_DX	(HW.pDevice->Clear(1,&R,D3DCLEAR_TARGET,D3DCOLOR_XRGB(32,32,32),1,0));
#endif	//	USE_DX10
}