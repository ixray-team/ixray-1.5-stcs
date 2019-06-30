#pragma once

#include "../shader_xrlc.h"
#include "../../xrcore/xrPool.h"
//#include "xrface.h"
#include "xrfacedefs.h"
#include "xrdeflectordefs.h"
#include "b_build_texture.h"
#include "base_lighting.h"

namespace CDB{
class MODEL;
class CollectorPacked;
};
class CLightmap;
class	XRLC_LIGHT_API xrLC_GlobalData
{
	
		xr_vector<b_BuildTexture>		_textures;
		xr_vector<b_material>			_materials;
		Shader_xrLC_LIB					_shaders;				
		CMemoryWriter					_err_invalid;
		b_params						_g_params;
		xr_vector<CLightmap*>			_g_lightmaps;
		vecVertex						_g_vertices;
		vecFace							_g_faces;
		vecDefl							_g_deflectors;
		base_lighting					_L_static;
		CDB::MODEL*						_RCAST_Model;
		bool							_b_nosun;
		bool							_gl_linear;
public:
		bool							b_reading;
		bool							b_writing;
public:
									xrLC_GlobalData	():_RCAST_Model (0), _b_nosun(false),_gl_linear(false),b_reading(false),b_writing(false) {}

		IC xr_vector<b_BuildTexture>& textures		()		{	return _textures; }
		IC xr_vector<CLightmap*>	& lightmaps		()		{	return _g_lightmaps; }
		IC xr_vector<b_material>	& materials		()		{	return _materials; }
		IC Shader_xrLC_LIB			& shaders		()		{	return _shaders; }
		IC CMemoryWriter			&err_invalid	()		{	return _err_invalid; }
		IC b_params					&g_params		()		{	return _g_params; }
			
		Face						*create_face	()		;
		void						destroy_face	(Face* &f );

		Vertex						*create_vertex	()		;
		void						destroy_vertex	(Vertex* &f );

		vecVertex					&g_vertices		()		{	return	_g_vertices; }
		vecFace						&g_faces		()		{	return	_g_faces; }
		vecDefl						&g_deflectors	()		{	return	_g_deflectors; }

		base_lighting				&L_static		()		{	return _L_static; }
		CDB::MODEL*					RCAST_Model		()		{	return _RCAST_Model; }
		bool						b_nosun			()		{	return _b_nosun; }
		bool						gl_linear		()		{	return _gl_linear; }
IC		void						b_nosun_set		(bool v){	_b_nosun = v; }
		void						initialize		()		;
		void						destroy_rcmodel	()		;
		void						create_rcmodel	(CDB::CollectorPacked& CL);

public:
		u32							get_id			( Face* f );
		u32							get_id			( Vertex* v );
		void						read			( IReader	&r );
		void						write			( IWriter	&w ) const ;
};													

extern "C" XRLC_LIGHT_API xrLC_GlobalData*	lc_global_data();
extern "C" XRLC_LIGHT_API void				create_global_data();
extern "C" XRLC_LIGHT_API void				destroy_global_data();
extern "C" XRLC_LIGHT_API u32				InvalideFaces();