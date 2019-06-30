#include "stdafx.h"
#include "build.h"
#include "../xrLC_Light/xrface.h"
#include "calculate_normals.h"
#include "../xrLC_Light/xrLC_GlobalData.h"


bool g_using_smooth_groups = true;
bool g_smooth_groups_by_faces = false;
//void 
// Performs simple cross-smooth

void CBuild::CalcNormals()
{

	calculate_normals<Vertex>::calc_normals( lc_global_data()->g_vertices(), lc_global_data()->g_faces() );
	// Models
	Status	("Models...");
	MU_ModelsCalculateNormals();
}

