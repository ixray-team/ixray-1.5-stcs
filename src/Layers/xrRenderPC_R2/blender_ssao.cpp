#include "stdafx.h"
#pragma hdrstop

#include "Blender_SSAO.h"

CBlender_SSAO::CBlender_SSAO	()	{	description.CLS		= 0;	}
CBlender_SSAO::~CBlender_SSAO	()	{	}

void	CBlender_SSAO::Compile			(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	switch (C.iElement)
	{
	case 0:		// calculate SSAO
		C.r_Pass			("combine_1",		"ssao_calc",	FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		jitter(C);
		C.r_End				();
		break;
	}
}
