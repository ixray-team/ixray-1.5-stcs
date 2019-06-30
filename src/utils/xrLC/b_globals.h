#pragma once


const	u32								c_VB_maxVertices		= 65535;		// count
const	u32								c_vCacheSize			= 24;			// entries
const	u32								c_SS_LowVertLimit		= 64;			// polys
const	u32								c_SS_HighVertLimit		= 2*1024;		// polys
const	u32								c_SS_maxsize			= 32;			// meters
const	u32								c_PM_FaceLimit			= 128;			// face-limit
const	float							c_PM_MetricLimit_static	= 0.10f;		// vertex-count-simplification-limit
const	float							c_PM_MetricLimit_mu		= 0.05f;		// vertex-count-simplification-limit

BOOL	exact_normalize					(Fvector3&	a);
BOOL	exact_normalize					(float*		a);


//#include "../xrLC_Light/xrFace.h"
//#include "../xrLC_Light/xrDeflector.h"
//#include "vbm.h"
//#include "OGF_Face.h"



static const int	edge2idx3	[3][3]	= { {0,1,2},	{1,2,0},	{2,0,1}	};
static const int	edge2idx	[3][2]	= { {0,1},		{1,2},		{2,0}	};
static const int	idx2edge	[3][3]  = {
	{-1,  0,  2},
	{ 0, -1,  1},
	{ 2,  1, -1}
};


extern BOOL						b_radiosity;
extern BOOL						b_noise;

