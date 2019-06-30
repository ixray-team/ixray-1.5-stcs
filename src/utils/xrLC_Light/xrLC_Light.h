#pragma once


#ifdef XRLC_LIGHT_EXPORTS
#	define XRLC_LIGHT_API __declspec(dllexport)
#else
#	define XRLC_LIGHT_API __declspec(dllimport)
#endif

#pragma warning(disable:4995)
#include <commctrl.h>
#include <d3dx9.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#pragma warning(default:4995)

#include "../../xrCore/xrCore.h"
//#include "cl_log.h"
#include "_d3d_extensions.h"
#include "../../editors/LevelEditor/Engine/communicate.h"
#include "Etextureparams.h"

extern XRLC_LIGHT_API xr_pure_interface  XRLC_LIGHT_API i_lc_log 
{
	virtual void clMsg		( LPCSTR msg )			=0;
	virtual void Status		( LPCSTR msg )			=0;
	virtual	void Progress	( const float F )		=0;
	virtual	void Phase		( LPCSTR phase_name )	=0;
} *lc_log;


extern "C" XRLC_LIGHT_API  b_params	&g_params();

IC	u8	u8_clr				(float a)	{ s32 _a = iFloor(a*255.f); clamp(_a,0,255); return u8(_a);		};

#ifndef	NDEBUG
#define X_TRY 
#define X_CATCH if (0)
#else
#define X_TRY try
#define X_CATCH catch(...)
#endif