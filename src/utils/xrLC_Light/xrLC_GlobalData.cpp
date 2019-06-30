#include "stdafx.h"

#include "xrLC_GlobalData.h"
#include "xrface.h"
#include "xrdeflector.h"
#include "lightmap.h"
#include "serialize.h"

#include "../../xrcdb/xrcdb.h"


xrLC_GlobalData* data =0;

tread_lightmaps		*read_lightmaps		= 0;
twrite_lightmaps	*write_lightmaps	= 0;

twrite_faces		*write_faces		= 0;
tread_faces			*read_faces			= 0;
tread_vertices		*read_vertices		= 0;
twrite_vertices		*write_vertices		= 0;
tread_deflectors	*read_deflectors	= 0;
twrite_deflectors	*write_deflectors	= 0;


 xrLC_GlobalData*	lc_global_data()
 {
	 return data;
 }
void	create_global_data()
{
	VERIFY( !lc_global_data() );
	data = xr_new<xrLC_GlobalData>();
}
void	destroy_global_data()
{
	VERIFY( lc_global_data() );
	xr_delete(data);
}

/*
poolVertices &xrLC_GlobalData	::VertexPool	()		
{
	return	_VertexPool; 
}
poolFaces &xrLC_GlobalData	::FacePool			()		
{
	return	_FacePool;
}
*/




void	xrLC_GlobalData	::destroy_rcmodel	()
{
	xr_delete		(_RCAST_Model);
}
void	xrLC_GlobalData	::create_rcmodel	(CDB::CollectorPacked& CL)
{
	_RCAST_Model				= xr_new<CDB::MODEL> ();
	_RCAST_Model->build		(CL.getV(),(int)CL.getVS(),CL.getT(),(int)CL.getTS());
}

void		xrLC_GlobalData	::				initialize		()
{
	if (strstr(Core.Params,"-att"))	_gl_linear	= true;
}


/*
		xr_vector<b_BuildTexture>		_textures;
		xr_vector<b_material>			_materials;
		Shader_xrLC_LIB					_shaders;				
		CMemoryWriter					_err_invalid;
		b_params						_g_params;
		vecVertex						_g_vertices;
		vecFace							_g_faces;
		vecDefl							_g_deflectors;
		base_lighting					_L_static;
		CDB::MODEL*						_RCAST_Model;
		bool							_b_nosun;
		bool							_gl_linear;
*/

void read( IReader	&r, CDB::MODEL* &m )
{
	VERIFY(!m);
	xr_vector<Fvector> verts;
	xr_vector<CDB::TRI> tris;
	r_pod_vector( r, verts );
	r_pod_vector( r, tris );
	m = xr_new<CDB::MODEL> ();
	m->build( &*verts.begin(), (int)verts.size(), &*tris.begin(), (int)tris.size() );

}
void write( IWriter	&w,  CDB::MODEL &m )
{
	w.w_u32( (u32)m.get_verts_count() );
	w.w( m.get_verts(), m.get_verts_count() * sizeof(Fvector) );
	w.w_u32( (u32)m.get_tris_count() );
	w.w( m.get_tris(), m.get_tris_count() * sizeof(CDB::TRI) );
}

void		xrLC_GlobalData	::read			( IReader	&r )
{
	r_vector( r, _textures );
	r_pod_vector( r, _materials );
	r_pod_vector( r, _shaders.Library	() );	
	//	CMemoryWriter					_err_invalid;
	r_pod( r, _g_params );

	read_lightmaps= xr_new< tread_lightmaps >( &_g_lightmaps );
	read_lightmaps->read( r );

	read_vertices = xr_new< tread_vertices	>( &_g_vertices );
	read_vertices->read( r );
	read_faces = xr_new< tread_faces	>( &_g_faces );
	read_faces->read( r );
	read_deflectors = xr_new< tread_deflectors	>( &_g_deflectors );
	read_deflectors->read( r );
	_L_static.read( r );
	::read( r, _RCAST_Model );
	_b_nosun = !!r.r_u8();
	_gl_linear = !!r.r_u8();

	xr_delete( read_lightmaps );
	xr_delete( read_vertices );
	xr_delete( read_faces );
	xr_delete( read_deflectors );

}
void		xrLC_GlobalData	::				write			( IWriter	&w ) const
{
	w_vector( w, _textures );
	w_pod_vector( w, _materials );
	w_pod_vector( w, _shaders.Library	() );	
	//	CMemoryWriter					_err_invalid;
	w_pod( w, _g_params );

	write_lightmaps= xr_new< twrite_lightmaps >( &_g_lightmaps );
	write_lightmaps->write( w );

	write_vertices = xr_new< twrite_vertices	>( &(_g_vertices) );
	write_vertices->write( w );
	write_faces = xr_new< twrite_faces	>( &_g_faces );
	write_faces->write( w );
	write_deflectors = xr_new< twrite_deflectors	>( &_g_deflectors );
	write_deflectors->write( w );
	_L_static.write( w );
	::write( w, *_RCAST_Model);
	w.w_u8(_b_nosun);
	w.w_u8(_gl_linear);
	
	xr_delete( write_lightmaps );
	xr_delete( write_vertices );
	xr_delete( write_faces );
	xr_delete( write_deflectors );

}