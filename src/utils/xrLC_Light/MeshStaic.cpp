#include "stdafx.h"

#include "xrLC_GlobalData.h"

#include "xrface.h"


typedef poolSS<Vertex,8*1024>	poolVertices;
typedef poolSS<Face,8*1024>		poolFaces;
static poolVertices	_VertexPool;
static poolFaces	_FacePool;

Face* xrLC_GlobalData	::create_face	()		
{
	return _FacePool.create();
}
void xrLC_GlobalData	::destroy_face	(Face* &f)
{
	_FacePool.destroy( f );
}

Vertex* xrLC_GlobalData	::create_vertex	()		
{
	return _VertexPool.create();
}
void xrLC_GlobalData	::destroy_vertex	(Vertex* &f)
{
	_VertexPool.destroy( f );
}




u32			xrLC_GlobalData	::get_id			( Face* f )
{
	/*
	if(b_writting)
	{
		vecFaceIt F = std::find( g_faces().begin(), g_faces().end(), f );
		if( F == g_faces().end() )
			return u32(-1);
		return F - g_faces().begin();
	}
	
*/
	return 0;
}
u32			xrLC_GlobalData	::get_id			( Vertex* v )
{
	return 0;
}




