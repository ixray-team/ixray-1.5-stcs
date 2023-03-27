#include "stdafx.h"
#include "r3.h"
#include "../xrRender/fbasicvisual.h"
#include "../../xrEngine/xr_object.h"
#include "../../xrEngine/CustomHUD.h"
#include "../../xrEngine/igame_persistent.h"
#include "../../xrEngine/environment.h"
#include "../xrRender/SkeletonCustom.h"
#include "../xrRender/LightTrack.h"
#include "../xrRender/dxRenderDeviceRender.h"
#include "../xrRender/dxWallMarkArray.h"
#include "../xrRender/dxUIShader.h"

#include "..\xrRenderDX10\3DFluid\dx103DFluidManager.h"

#include "D3D10shader.h"

CRender										RImplementation;

//////////////////////////////////////////////////////////////////////////
class CGlow				: public IRender_Glow
{
public:
	bool				bActive;
public:
	CGlow() : bActive(false)		{ }
	virtual void					set_active			(bool b)					{ bActive=b;		}
	virtual bool					get_active			()							{ return bActive;	}
	virtual void					set_position		(const Fvector& P)			{ }
	virtual void					set_direction		(const Fvector& D)			{ }
	virtual void					set_radius			(float R)					{ }
	virtual void					set_texture			(LPCSTR name)				{ }
	virtual void					set_color			(const Fcolor& C)			{ }
	virtual void					set_color			(float r, float g, float b)	{ }
};

float		r_dtex_range		= 50.f;
//////////////////////////////////////////////////////////////////////////
ShaderElement*			CRender::rimp_select_sh_dynamic	(dxRender_Visual	*pVisual, float cdist_sq)
{
	int		id	= SE_R2_SHADOW;
	if	(CRender::PHASE_NORMAL == RImplementation.phase)
	{
		id = ((_sqrt(cdist_sq)-pVisual->vis.sphere.R)<r_dtex_range)?SE_R2_NORMAL_HQ:SE_R2_NORMAL_LQ;
	}
	return pVisual->shader->E[id]._get();
}
//////////////////////////////////////////////////////////////////////////
ShaderElement*			CRender::rimp_select_sh_static	(dxRender_Visual	*pVisual, float cdist_sq)
{
	int		id	= SE_R2_SHADOW;
	if	(CRender::PHASE_NORMAL == RImplementation.phase)
	{
		id = ((_sqrt(cdist_sq)-pVisual->vis.sphere.R)<r_dtex_range)?SE_R2_NORMAL_HQ:SE_R2_NORMAL_LQ;
	}
	return pVisual->shader->E[id]._get();
}
static class cl_parallax		: public R_constant_setup		{	virtual void setup	(R_constant* C)
{
	float			h			=	ps_r2_df_parallax_h;
	RCache.set_c	(C,h,-h/2.f,1.f/r_dtex_range,1.f/r_dtex_range);
}}	binder_parallax;

static class cl_pos_decompress_params		: public R_constant_setup		{	virtual void setup	(R_constant* C)
{
	float VertTan =  -1.0f * tanf( deg2rad(Device.fFOV/2.0f ) );
	float HorzTan =  - VertTan / Device.fASPECT;

	RCache.set_c	( C, HorzTan, VertTan, ( 2.0f * HorzTan )/(float)Device.dwWidth, ( 2.0f * VertTan ) /(float)Device.dwHeight );

}}	binder_pos_decompress_params;

static class cl_pos_decompress_params2		: public R_constant_setup		{	virtual void setup	(R_constant* C)
{
	RCache.set_c	(C,(float)Device.dwWidth, (float)Device.dwHeight, 1.0f/(float)Device.dwWidth, 1.0f/(float)Device.dwHeight );

}}	binder_pos_decompress_params2;

static class cl_water_intensity : public R_constant_setup		
{	
	virtual void setup	(R_constant* C)
	{
		CEnvDescriptor&	E = *g_pGamePersistent->Environment().CurrentEnv;
		float fValue = E.m_fWaterIntensity;
		RCache.set_c	(C, fValue, fValue, fValue, 0);
	}
}	binder_water_intensity;

static class cl_sun_shafts_intensity : public R_constant_setup		
{	
	virtual void setup	(R_constant* C)
	{
		CEnvDescriptor&	E = *g_pGamePersistent->Environment().CurrentEnv;
		float fValue = E.m_fSunShaftsIntensity;
		RCache.set_c	(C, fValue, fValue, fValue, 0);
	}
}	binder_sun_shafts_intensity;

static class cl_alpha_ref	: public R_constant_setup 
{	
	virtual void setup (R_constant* C) 
	{ 
		StateManager.BindAlphaRef(C);
	}
} binder_alpha_ref;

extern ENGINE_API BOOL r2_advanced_pp;	//	advanced post process and effects
//////////////////////////////////////////////////////////////////////////
// Just two static storage
void					CRender::create					()
{
	Device.seqFrame.Add	(this,REG_PRIORITY_HIGH+0x12345678);

	m_skinning			= -1;
	m_MSAASample		= -1;

	// hardware
	o.smapsize			= 2048;
	o.mrt				= (HW.Caps.raster.dwMRT_count >= 3);
	o.mrtmixdepth		= (HW.Caps.raster.b_MRT_mixdepth);

	// Check for NULL render target support
	//	DX10 disabled
	//D3DFORMAT	nullrt	= (D3DFORMAT)MAKEFOURCC('N','U','L','L');
	//o.nullrt			= HW.support	(nullrt,			D3DRTYPE_SURFACE, D3DUSAGE_RENDERTARGET);
	o.nullrt = false;
	/*
	if (o.nullrt)		{
	Msg				("* NULLRT supported and used");
	};
	*/
	if (o.nullrt)		{
		Msg				("* NULLRT supported");

		//.	    _tzset			();
		//.		??? _strdate	( date, 128 );	???
		//.		??? if (date < 22-march-07)		
		if (0)
		{
			u32 device_id	= HW.Caps.id_device;
			bool disable_nullrt = false;
			switch (device_id)	
			{
			case 0x190:
			case 0x191:
			case 0x192:
			case 0x193:
			case 0x194:
			case 0x197:
			case 0x19D:
			case 0x19E:{
				disable_nullrt = true;	//G80
				break;
					   }
			case 0x400:
			case 0x401:
			case 0x402:
			case 0x403:
			case 0x404:
			case 0x405:
			case 0x40E:
			case 0x40F:{
				disable_nullrt = true;	//G84
				break;
					   }
			case 0x420:
			case 0x421:
			case 0x422:
			case 0x423:
			case 0x424:
			case 0x42D:
			case 0x42E:
			case 0x42F:{
				disable_nullrt = true;	// G86
				break;
					   }
			}
			if (disable_nullrt)	o.nullrt=false;
		};
		if (o.nullrt)	Msg				("* ...and used");
	};


	// SMAP / DST
	o.HW_smap_FETCH4	= FALSE;
	//	DX10 disabled
	//o.HW_smap			= HW.support	(D3DFMT_D24X8,			D3DRTYPE_TEXTURE,D3DUSAGE_DEPTHSTENCIL);
	o.HW_smap			= true;
	o.HW_smap_PCF		= o.HW_smap		;
	if (o.HW_smap)		
	{
		//	For ATI it's much faster on DX10 to use D32F format
		if (HW.Caps.id_vendor==0x1002)
			o.HW_smap_FORMAT	= D3DFMT_D32F_LOCKABLE;
		else
			o.HW_smap_FORMAT	= D3DFMT_D24X8;
		Msg				("* HWDST/PCF supported and used");
	}

	//	DX10 disabled
	//o.fp16_filter		= HW.support	(D3DFMT_A16B16G16R16F,	D3DRTYPE_TEXTURE,D3DUSAGE_QUERY_FILTER);
	//o.fp16_blend		= HW.support	(D3DFMT_A16B16G16R16F,	D3DRTYPE_TEXTURE,D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING);
	o.fp16_filter		= true;
	o.fp16_blend		= true;

	// search for ATI formats
	if (!o.HW_smap && (0==strstr(Core.Params,"-nodf24")) )		{
		o.HW_smap		= HW.support	((D3DFORMAT)(MAKEFOURCC('D','F','2','4')),	D3DRTYPE_TEXTURE,D3DUSAGE_DEPTHSTENCIL);
		if (o.HW_smap)	{
			o.HW_smap_FORMAT= MAKEFOURCC	('D','F','2','4');
			o.HW_smap_PCF	= FALSE			;
			o.HW_smap_FETCH4= TRUE			;
		}
		Msg				("* DF24/F4 supported and used [%X]", o.HW_smap_FORMAT);
	}

	// emulate ATI-R4xx series
	if (strstr(Core.Params,"-r4xx"))	{
		o.mrtmixdepth	= FALSE;
		o.HW_smap		= FALSE;
		o.HW_smap_PCF	= FALSE;
		o.fp16_filter	= FALSE;
		o.fp16_blend	= FALSE;
	}

	VERIFY2				(o.mrt && (HW.Caps.raster.dwInstructions>=256),"Hardware doesn't meet minimum feature-level");
	if (o.mrtmixdepth)		o.albedo_wo		= FALSE	;
	else if (o.fp16_blend)	o.albedo_wo		= FALSE	;
	else					o.albedo_wo		= TRUE	;

	// nvstencil on NV40 and up
	o.nvstencil			= FALSE;
	//if ((HW.Caps.id_vendor==0x10DE)&&(HW.Caps.id_device>=0x40))	o.nvstencil = TRUE;
	if (strstr(Core.Params,"-nonvs"))		o.nvstencil	= FALSE;

	// nv-dbt
	//	DX10 disabled
	//o.nvdbt				= HW.support	((D3DFORMAT)MAKEFOURCC('N','V','D','B'), D3DRTYPE_SURFACE, 0);
	o.nvdbt				= false;
	if (o.nvdbt)		Msg	("* NV-DBT supported and used");

	// options (smap-pool-size)
	if (strstr(Core.Params,"-smap1536"))	o.smapsize	= 1536;
	if (strstr(Core.Params,"-smap2048"))	o.smapsize	= 2048;
	if (strstr(Core.Params,"-smap2560"))	o.smapsize	= 2560;
	if (strstr(Core.Params,"-smap3072"))	o.smapsize	= 3072;
	if (strstr(Core.Params,"-smap4096"))	o.smapsize	= 4096;

	// gloss
	char*	g			= strstr(Core.Params,"-gloss ");
	o.forcegloss		= g?	TRUE	:FALSE	;
	if (g)				{
		o.forcegloss_v		= float	(atoi	(g+xr_strlen("-gloss ")))/255.f;
	}

	// options
	o.bug				= (strstr(Core.Params,"-bug"))?			TRUE	:FALSE	;
	o.sunfilter			= (strstr(Core.Params,"-sunfilter"))?	TRUE	:FALSE	;
	o.sunstatic = !ps_r2_ls_flags.test(R2FLAG_SUN) ? TRUE : FALSE;
	o.advancedpp		= r2_advanced_pp;
	o.volumetricfog		= ps_r2_ls_flags.test(R3FLAG_VOLUMETRIC_SMOKE);
	o.sjitter			= (strstr(Core.Params,"-sjitter"))?		TRUE	:FALSE	;
	o.depth16			= (strstr(Core.Params,"-depth16"))?		TRUE	:FALSE	;
	o.noshadows			= (strstr(Core.Params,"-noshadows"))?	TRUE	:FALSE	;
	o.Tshadows			= (strstr(Core.Params,"-tsh"))?			TRUE	:FALSE	;
	o.mblur				= (strstr(Core.Params,"-mblur"))?		TRUE	:FALSE	;
	o.distortion_enabled= (strstr(Core.Params,"-nodistort"))?	FALSE	:TRUE	;
	o.distortion		= o.distortion_enabled;
	o.disasm			= (strstr(Core.Params,"-disasm"))?		TRUE	:FALSE	;
	o.forceskinw		= (strstr(Core.Params,"-skinw"))?		TRUE	:FALSE	;

	o.ssao_blur_on		= ps_r2_ls_flags_ext.test(R2FLAGEXT_SSAO_BLUR) && (ps_r_ssao != 0);
	o.ssao_opt_data		= ps_r2_ls_flags_ext.test(R2FLAGEXT_SSAO_OPT_DATA) && (ps_r_ssao != 0);
	o.ssao_half_data	= ps_r2_ls_flags_ext.test(R2FLAGEXT_SSAO_HALF_DATA) && o.ssao_opt_data && (ps_r_ssao != 0);
	o.ssao_hdao			= ps_r2_ls_flags_ext.test(R2FLAGEXT_SSAO_HDAO) && (ps_r_ssao != 0);
	o.ssao_hbao			= !o.ssao_hdao && ps_r2_ls_flags_ext.test(R2FLAGEXT_SSAO_HBAO) && (ps_r_ssao != 0);

	//	TODO: fix hbao shader to allow to perform per-subsample effect!
	if (o.ssao_hbao || o.ssao_hdao)
		o.ssao_opt_data = true;

	o.dx10_sm4_1		= ps_r2_ls_flags.test((u32)R3FLAG_USE_DX10_1);
	o.dx10_sm4_1		= o.dx10_sm4_1 && ( HW.pDevice1 != 0 );

	//	MSAA option dependencies

	o.dx10_msaa			= !!ps_r3_msaa;
	o.dx10_msaa_samples = (1 << ps_r3_msaa);

	o.dx10_msaa_opt		= ps_r2_ls_flags.test(R3FLAG_MSAA_OPT);
	o.dx10_msaa_opt		= o.dx10_msaa_opt && o.dx10_msaa && ( HW.pDevice1 != 0 );

	//o.dx10_msaa_hybrid	= ps_r2_ls_flags.test(R3FLAG_MSAA_HYBRID);
	o.dx10_msaa_hybrid	= ps_r2_ls_flags.test((u32)R3FLAG_USE_DX10_1);
	o.dx10_msaa_hybrid	&= !o.dx10_msaa_opt && o.dx10_msaa && ( HW.pDevice1 != 0) ;

	//	Allow alpha test MSAA for DX10.0

	//o.dx10_msaa_alphatest= ps_r2_ls_flags.test((u32)R3FLAG_MSAA_ALPHATEST);
	//o.dx10_msaa_alphatest= o.dx10_msaa_alphatest && o.dx10_msaa;

	//o.dx10_msaa_alphatest_atoc= (o.dx10_msaa_alphatest && !o.dx10_msaa_opt && !o.dx10_msaa_hybrid);

	o.dx10_msaa_alphatest = 0;
	if (o.dx10_msaa)
	{
		if ( o.dx10_msaa_opt || o.dx10_msaa_hybrid )
		{
			if (ps_r3_msaa_atest==1)
				o.dx10_msaa_alphatest = MSAA_ATEST_DX10_1_ATOC;
			else if (ps_r3_msaa_atest==2)
				o.dx10_msaa_alphatest = MSAA_ATEST_DX10_1_NATIVE;
		}
		else
		{
			if (ps_r3_msaa_atest)
				o.dx10_msaa_alphatest = MSAA_ATEST_DX10_0_ATOC;
		}
	}

	o.dx10_gbuffer_opt	= ps_r2_ls_flags.test(R3FLAG_GBUFFER_OPT);

	o.dx10_minmax_sm = ps_r3_minmax_sm;
	o.dx10_minmax_sm_screenarea_threshold = 1600*1200;

	if (o.dx10_minmax_sm==MMSM_AUTODETECT)
	{
		o.dx10_minmax_sm = MMSM_OFF;

		//	AMD device
		if (HW.Caps.id_vendor==0x1002)
		{
			if (ps_r_sun_quality>=3)
				o.dx10_minmax_sm=MMSM_AUTO;
			else if (ps_r_sun_shafts>=2)
			{
				o.dx10_minmax_sm=MMSM_AUTODETECT;
				//	Check resolution in runtime in use_minmax_sm_this_frame
				o.dx10_minmax_sm_screenarea_threshold = 1600*1200;
			}
		}

		//	NVidia boards
		if (HW.Caps.id_vendor==0x10DE)
		{
			if ((ps_r_sun_shafts>=2))
			{
				o.dx10_minmax_sm=MMSM_AUTODETECT;
				//	Check resolution in runtime in use_minmax_sm_this_frame
				o.dx10_minmax_sm_screenarea_threshold = 1280*1024;
			}
		}
	}

	// constants
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("parallax",	&binder_parallax);
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("water_intensity",	&binder_water_intensity);
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("sun_shafts_intensity",	&binder_sun_shafts_intensity);
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("m_AlphaRef",	&binder_alpha_ref);
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("pos_decompression_params",	&binder_pos_decompress_params);
	dxRenderDeviceRender::Instance().Resources->RegisterConstantSetup	("pos_decompression_params2",	&binder_pos_decompress_params2);

	c_lmaterial					= "L_material";
	c_sbase						= "s_base";

	m_bMakeAsyncSS				= false;

	Target						= xr_new<CRenderTarget>		();	// Main target

	Models						= xr_new<CModelPool>		();
	PSLibrary.OnCreate			();
	HWOCC.occq_create			(occq_size);

	rmNormal					();
	marker						= 0;
	D3D10_QUERY_DESC			qdesc;
	qdesc.MiscFlags				= 0;
	qdesc.Query					= D3D10_QUERY_EVENT;
	ZeroMemory(q_sync_point, sizeof(q_sync_point));
	//R_CHK						(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[0]));
	//R_CHK						(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[1]));
	//	Prevent error on first get data
	//q_sync_point[0]->End();
	//q_sync_point[1]->End();
	//R_CHK						(HW.pDevice->CreateQuery(D3DQUERYTYPE_EVENT,&q_sync_point[0]));
	//R_CHK						(HW.pDevice->CreateQuery(D3DQUERYTYPE_EVENT,&q_sync_point[1]));
	for (u32 i=0; i<HW.Caps.iGPUNum; ++i)
		R_CHK(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[i]));
	q_sync_point[0]->End();

	xrRender_apply_tf			();
	::PortalTraverser.initialize();
	FluidManager.Initialize( 70, 70, 70 );
//	FluidManager.Initialize( 100, 100, 100 );
	FluidManager.SetScreenSize(Device.dwWidth, Device.dwHeight);
}

void					CRender::destroy				()
{
	m_bMakeAsyncSS				= false;
	FluidManager.Destroy();
	::PortalTraverser.destroy	();
	//_RELEASE					(q_sync_point[1]);
	//_RELEASE					(q_sync_point[0]);
	for (u32 i=0; i<HW.Caps.iGPUNum; ++i)
		_RELEASE				(q_sync_point[i]);
	
	HWOCC.occq_destroy			();
	xr_delete					(Models);
	xr_delete					(Target);
	PSLibrary.OnDestroy			();
	Device.seqFrame.Remove		(this);
	r_dsgraph_destroy();
}

void CRender::reset_begin()
{
	// Update incremental shadowmap-visibility solver
	// BUG-ID: 10646
	{
		u32 it=0;
		for (it=0; it<Lights_LastFrame.size(); it++)	{
			if (0==Lights_LastFrame[it])	continue	;
			try {
				Lights_LastFrame[it]->svis.resetoccq ()	;
			} catch (...)
			{
				Msg	("! Failed to flush-OCCq on light [%d] %X",it,*(u32*)(&Lights_LastFrame[it]));
			}
		}
		Lights_LastFrame.clear	();
	}

	xr_delete					(Target);
	HWOCC.occq_destroy			();
	//_RELEASE					(q_sync_point[1]);
	//_RELEASE					(q_sync_point[0]);
	for (u32 i=0; i<HW.Caps.iGPUNum; ++i)
		_RELEASE				(q_sync_point[i]);
}

void CRender::reset_end()
{
	D3D10_QUERY_DESC			qdesc;
	qdesc.MiscFlags				= 0;
	qdesc.Query					= D3D10_QUERY_EVENT;
	//R_CHK						(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[0]));
	//R_CHK						(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[1]));
	for (u32 i=0; i<HW.Caps.iGPUNum; ++i)
		R_CHK(HW.pDevice->CreateQuery(&qdesc,&q_sync_point[i]));
	//	Prevent error on first get data
	q_sync_point[0]->End();
	//q_sync_point[1]->End();
	//R_CHK						(HW.pDevice->CreateQuery(D3DQUERYTYPE_EVENT,&q_sync_point[0]));
	//R_CHK						(HW.pDevice->CreateQuery(D3DQUERYTYPE_EVENT,&q_sync_point[1]));
	HWOCC.occq_create			(occq_size);

	Target						=	xr_new<CRenderTarget>	();

	xrRender_apply_tf			();
	FluidManager.SetScreenSize(Device.dwWidth, Device.dwHeight);

	// Set this flag true to skip the first render frame,
	// that some data is not ready in the first frame (for example device camera position)
	m_bFirstFrameAfterReset = true;
}
/*
void CRender::OnFrame()
{
	Models->DeleteQueue			();
	if (ps_r2_ls_flags.test(R2FLAG_EXP_MT_CALC))	{
		Device.seqParallel.insert	(Device.seqParallel.begin(),
			fastdelegate::FastDelegate0<>(&HOM,&CHOM::MT_RENDER));
	}
}*/
void CRender::OnFrame()
{
	Models->DeleteQueue			();
	if (ps_r2_ls_flags.test(R2FLAG_EXP_MT_CALC))	{
		// MT-details (@front)
		Device.seqParallel.insert	(Device.seqParallel.begin(),
			fastdelegate::FastDelegate0<>(Details,&CDetailManager::MT_CALC));

		// MT-HOM (@front)
		Device.seqParallel.insert	(Device.seqParallel.begin(),
			fastdelegate::FastDelegate0<>(&HOM,&CHOM::MT_RENDER));
	}
}


// Implementation
IRender_ObjectSpecific*	CRender::ros_create				(IRenderable* parent)				{ return xr_new<CROS_impl>();			}
void					CRender::ros_destroy			(IRender_ObjectSpecific* &p)		{ xr_delete(p);							}
IRenderVisual*			CRender::model_Create			(LPCSTR name, IReader* data)		{ return Models->Create(name,data);		}
IRenderVisual*			CRender::model_CreateChild		(LPCSTR name, IReader* data)		{ return Models->CreateChild(name,data);}
IRenderVisual*			CRender::model_Duplicate		(IRenderVisual* V)					{ return Models->Instance_Duplicate((dxRender_Visual*)V);	}
void					CRender::model_Delete			(IRenderVisual* &V, BOOL bDiscard)	
{ 
	dxRender_Visual* pVisual = (dxRender_Visual*)V;
	Models->Delete(pVisual, bDiscard);
	V = 0;
}
IRender_DetailModel*	CRender::model_CreateDM			(IReader*	F)
{
	CDetail*	D		= xr_new<CDetail> ();
	D->Load				(F);
	return D;
}
void					CRender::model_Delete			(IRender_DetailModel* & F)
{
	if (F)
	{
		CDetail*	D	= (CDetail*)F;
		D->Unload		();
		xr_delete		(D);
		F				= NULL;
	}
}
IRenderVisual*			CRender::model_CreatePE			(LPCSTR name)	
{ 
	PS::CPEDef*	SE			= PSLibrary.FindPED	(name);		R_ASSERT3(SE,"Particle effect doesn't exist",name);
	return					Models->CreatePE	(SE);
}
IRenderVisual*			CRender::model_CreateParticles	(LPCSTR name)	
{ 
	PS::CPEDef*	SE			= PSLibrary.FindPED	(name);
	if (SE) return			Models->CreatePE	(SE);
	else{
		PS::CPGDef*	SG		= PSLibrary.FindPGD	(name);		R_ASSERT3(SG,"Particle effect or group doesn't exist",name);
		return				Models->CreatePG	(SG);
	}
}
void					CRender::models_Prefetch		()					{ Models->Prefetch	();}
void					CRender::models_Clear			(BOOL b_complete)	{ Models->ClearPool	(b_complete);}

ref_shader				CRender::getShader				(int id)			{ VERIFY(id<int(Shaders.size()));	return Shaders[id];	}
IRender_Portal*			CRender::getPortal				(int id)			{ VERIFY(id<int(Portals.size()));	return Portals[id];	}
IRender_Sector*			CRender::getSector				(int id)			{ VERIFY(id<int(Sectors.size()));	return Sectors[id];	}
IRender_Sector*			CRender::getSectorActive		()					{ return pLastSector;									}
IRenderVisual*			CRender::getVisual				(int id)			{ VERIFY(id<int(Visuals.size()));	return Visuals[id];	}
D3DVERTEXELEMENT9*		CRender::getVB_Format			(int id, BOOL	_alt)	{ 
	if (_alt)	{ VERIFY(id<int(xDC.size()));	return xDC[id].begin();	}
	else		{ VERIFY(id<int(nDC.size()));	return nDC[id].begin(); }
}
ID3DVertexBuffer*	CRender::getVB					(int id, BOOL	_alt)	{
	if (_alt)	{ VERIFY(id<int(xVB.size()));	return xVB[id];		}
	else		{ VERIFY(id<int(nVB.size()));	return nVB[id];		}
}
ID3DIndexBuffer*	CRender::getIB					(int id, BOOL	_alt)	{ 
	if (_alt)	{ VERIFY(id<int(xIB.size()));	return xIB[id];		}
	else		{ VERIFY(id<int(nIB.size()));	return nIB[id];		}
}
FSlideWindowItem*		CRender::getSWI					(int id)			{ VERIFY(id<int(SWIs.size()));		return &SWIs[id];	}
IRender_Target*			CRender::getTarget				()					{ return Target;										}

IRender_Light*			CRender::light_create			()					{ return Lights.Create();								}
IRender_Glow*			CRender::glow_create			()					{ return xr_new<CGlow>();								}

void					CRender::flush					()					{ r_dsgraph_render_graph	(0);						}

BOOL					CRender::occ_visible			(vis_data& P)		{ return HOM.visible(P);								}
BOOL					CRender::occ_visible			(sPoly& P)			{ return HOM.visible(P);								}
BOOL					CRender::occ_visible			(Fbox& P)			{ return HOM.visible(P);								}

void					CRender::add_Visual				(IRenderVisual*		V )	{ add_leafs_Dynamic((dxRender_Visual*)V);								}
void					CRender::add_Geometry			(IRenderVisual*		V )	{ add_Static((dxRender_Visual*)V,View->getMask());					}
void					CRender::add_StaticWallmark		(ref_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* verts)
{
	if (T->suppress_wm)	return;
	VERIFY2							(_valid(P) && _valid(s) && T && verts && (s>EPS_L), "Invalid static wallmark params");
	Wallmarks->AddStaticWallmark	(T,verts,P,&*S,s);
}

void CRender::add_StaticWallmark			(IWallMarkArray *pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V)
{
	dxWallMarkArray *pWMA = (dxWallMarkArray *)pArray;
	ref_shader *pShader = pWMA->dxGenerateWallmark();
	if (pShader) add_StaticWallmark		(*pShader, P, s, T, V);
}

void CRender::add_StaticWallmark			(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V)
{
	dxUIShader* pShader = (dxUIShader*)&*S;
	add_StaticWallmark		(pShader->hShader, P, s, T, V);
}

void					CRender::clear_static_wallmarks	()
{
	Wallmarks->clear				();
}

void					CRender::add_SkeletonWallmark	(intrusive_ptr<CSkeletonWallmark> wm)
{
	Wallmarks->AddSkeletonWallmark				(wm);
}
void					CRender::add_SkeletonWallmark	(const Fmatrix* xf, CKinematics* obj, ref_shader& sh, const Fvector& start, const Fvector& dir, float size)
{
	Wallmarks->AddSkeletonWallmark				(xf, obj, sh, start, dir, size);
}
void					CRender::add_SkeletonWallmark	(const Fmatrix* xf, IKinematics* obj, IWallMarkArray *pArray, const Fvector& start, const Fvector& dir, float size)
{
	dxWallMarkArray *pWMA = (dxWallMarkArray *)pArray;
	ref_shader *pShader = pWMA->dxGenerateWallmark();
	if (pShader) add_SkeletonWallmark(xf, (CKinematics*)obj, *pShader, start, dir, size);
}
void					CRender::add_Occluder			(Fbox2&	bb_screenspace	)
{
	HOM.occlude			(bb_screenspace);
}
void					CRender::set_Object				(IRenderable*	O )	
{ 
	val_pObject				= O;
}
void					CRender::rmNear				()
{
	IRender_Target* T	=	getTarget	();
	D3D_VIEWPORT VP		=	{0,0,T->get_width(),T->get_height(),0,0.02f };

	HW.pDevice->RSSetViewports(1, &VP);
	//CHK_DX				(HW.pDevice->SetViewport(&VP));
}
void					CRender::rmFar				()
{
	IRender_Target* T	=	getTarget	();
	D3D_VIEWPORT VP		=	{0,0,T->get_width(),T->get_height(),0.99999f,1.f };

	HW.pDevice->RSSetViewports(1, &VP);
	//CHK_DX				(HW.pDevice->SetViewport(&VP));
}
void					CRender::rmNormal			()
{
	IRender_Target* T	=	getTarget	();
	D3D_VIEWPORT VP		= {0,0,T->get_width(),T->get_height(),0,1.f };

	HW.pDevice->RSSetViewports(1, &VP);
	//CHK_DX				(HW.pDevice->SetViewport(&VP));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRender::CRender() : m_bFirstFrameAfterReset(false)
{
	init_cacades();
}

CRender::~CRender()
{
}

#include "../../xrEngine/GameFont.h"
void	CRender::Statistics	(CGameFont* _F)
{
	CGameFont&	F	= *_F;
	F.OutNext	(" **** LT:%2d,LV:%2d **** ",stats.l_total,stats.l_visible		);	stats.l_visible = 0;
	F.OutNext	("    S(%2d)   | (%2d)NS   ",stats.l_shadowed,stats.l_unshadowed);
	F.OutNext	("smap use[%2d], merge[%2d], finalclip[%2d]",stats.s_used,stats.s_merged-stats.s_used,stats.s_finalclip);
	stats.s_used = 0; stats.s_merged = 0; stats.s_finalclip = 0;
	F.OutSkip	();
	F.OutNext	(" **** Occ-Q(%03.1f) **** ",100.f*f32(stats.o_culled)/f32(stats.o_queries?stats.o_queries:1));
	F.OutNext	(" total  : %2d",	stats.o_queries	);	stats.o_queries = 0;
	F.OutNext	(" culled : %2d",	stats.o_culled	);	stats.o_culled	= 0;
	F.OutSkip	();
	u32	ict		= stats.ic_total + stats.ic_culled;
	F.OutNext	(" **** iCULL(%03.1f) **** ",100.f*f32(stats.ic_culled)/f32(ict?ict:1));
	F.OutNext	(" visible: %2d",	stats.ic_total	);	stats.ic_total	= 0;
	F.OutNext	(" culled : %2d",	stats.ic_culled	);	stats.ic_culled	= 0;
#ifdef DEBUG
	HOM.stats	();
#endif
}

/////////
#pragma comment(lib,"d3dx9.lib")
/*
extern "C"
{
LPCSTR WINAPI	D3DXGetPixelShaderProfile	(LPDIRECT3DDEVICE9  pDevice);
LPCSTR WINAPI	D3DXGetVertexShaderProfile	(LPDIRECT3DDEVICE9	pDevice);
};
*/
HRESULT	CRender::shader_compile			(
	LPCSTR							name,
	LPCSTR                          pSrcData,
	UINT                            SrcDataLen,
	void*							_pDefines,
	void*							_pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pTarget,
	DWORD                           Flags,
	void*							_ppShader,
	void*							_ppErrorMsgs,
	void*							_ppConstantTable)
{
	D3D_SHADER_MACRO				defines			[128];
	int								def_it			= 0;
	CONST D3D_SHADER_MACRO*         pDefines		= (CONST D3D_SHADER_MACRO*)	_pDefines;
	char							c_smapsize		[32];
	char							c_gloss			[32];
	char							c_sun_shafts	[32];
	char							c_ssao			[32];
	char							c_sun_quality	[32];

	if (pDefines)	{
		// transfer existing defines
		for (;;def_it++)	{
			if (0==pDefines[def_it].Name)	break;
			defines[def_it]			= pDefines[def_it];
		}
	}
	// options
	{
		sprintf						(c_smapsize,"%d",u32(o.smapsize));
		defines[def_it].Name		=	"SMAP_size";
		defines[def_it].Definition	=	c_smapsize;
		def_it						++	;
	}
	if (o.fp16_filter)		{
		defines[def_it].Name		=	"FP16_FILTER";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.fp16_blend)		{
		defines[def_it].Name		=	"FP16_BLEND";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.HW_smap)			{
		defines[def_it].Name		=	"USE_HWSMAP";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.HW_smap_PCF)			{
		defines[def_it].Name		=	"USE_HWSMAP_PCF";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.HW_smap_FETCH4)			{
		defines[def_it].Name		=	"USE_FETCH4";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.sjitter)			{
		defines[def_it].Name		=	"USE_SJITTER";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (HW.Caps.raster_major >= 3)	{
		defines[def_it].Name		=	"USE_BRANCHING";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (HW.Caps.geometry.bVTF)	{
		defines[def_it].Name		=	"USE_VTF";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.Tshadows)			{
		defines[def_it].Name		=	"USE_TSHADOWS";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.mblur)			{
		defines[def_it].Name		=	"USE_MBLUR";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.sunfilter)		{
		defines[def_it].Name		=	"USE_SUNFILTER";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.sunstatic)		{
		defines[def_it].Name		=	"USE_R2_STATIC_SUN";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (o.forcegloss)		{
		sprintf						(c_gloss,"%f",o.forcegloss_v);
		defines[def_it].Name		=	"FORCE_GLOSS";
		defines[def_it].Definition	=	c_gloss;
		def_it						++	;
	}
	if (o.forceskinw)		{
		defines[def_it].Name		=	"SKIN_COLOR";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (o.ssao_blur_on)
	{
		defines[def_it].Name		=	"USE_SSAO_BLUR";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (o.ssao_opt_data)
	{
		defines[def_it].Name		=	"SSAO_OPT_DATA";
		if (o.ssao_half_data)
			defines[def_it].Definition	=	"2";
		else
			defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (o.ssao_hdao)
	{
		defines[def_it].Name		=	"HDAO";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (o.ssao_hbao)
	{
		defines[def_it].Name		=	"USE_HBAO";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if( o.dx10_msaa )
	{
		static char def[ 256 ];
      if( m_MSAASample < 0 )
         def[0]= '0';
      else
		   def[0]= '0' + char(m_MSAASample);
      def[1] = 0;
		defines[def_it].Name		=	"ISAMPLE";
		defines[def_it].Definition	=	def;
		def_it						++	;
	}

	// skinning
	if (m_skinning<0)		{
		defines[def_it].Name		=	"SKIN_NONE";
		defines[def_it].Definition	=	"1";
		def_it						++	;
	}
	if (0==m_skinning)		{
		defines[def_it].Name		=	"SKIN_0";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}
	if (1==m_skinning)		{
		defines[def_it].Name		=	"SKIN_1";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}
	if (2==m_skinning)		{
		defines[def_it].Name		=	"SKIN_2";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}
	if (3==m_skinning)		{
		defines[def_it].Name		=	"SKIN_3";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}
	if (4==m_skinning)		{
		defines[def_it].Name		=	"SKIN_4";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	//	Igor: need restart options
	if (RImplementation.o.advancedpp && ps_r2_ls_flags.test(R2FLAG_SOFT_WATER))
	{
		defines[def_it].Name		=	"USE_SOFT_WATER";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r2_ls_flags.test(R2FLAG_SOFT_PARTICLES))
	{
		defines[def_it].Name		=	"USE_SOFT_PARTICLES";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r2_ls_flags.test(R2FLAG_DOF))
	{
		defines[def_it].Name		=	"USE_DOF";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r_sun_shafts)
	{
		sprintf_s					(c_sun_shafts,"%d",ps_r_sun_shafts);
		defines[def_it].Name		=	"SUN_SHAFTS_QUALITY";
		defines[def_it].Definition	=	c_sun_shafts;
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r_ssao)
	{
		sprintf_s					(c_ssao,"%d",ps_r_ssao);
		defines[def_it].Name		=	"SSAO_QUALITY";
		defines[def_it].Definition	=	c_ssao;
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r_sun_quality)
	{
		sprintf_s					(c_sun_quality,"%d",ps_r_sun_quality);
		defines[def_it].Name		=	"SUN_QUALITY";
		defines[def_it].Definition	=	c_sun_quality;
		def_it						++;
	}

	if (RImplementation.o.advancedpp && ps_r2_ls_flags.test(R2FLAG_STEEP_PARALLAX))
	{
		defines[def_it].Name		=	"ALLOW_STEEPPARALLAX";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

   if( o.dx10_gbuffer_opt )
	{
		defines[def_it].Name		=	"GBUFFER_OPTIMIZATION";
		defines[def_it].Definition	=	"1";
		def_it						++;
	}

   if( o.dx10_sm4_1 )
   {
	   defines[def_it].Name		=	"SM_4_1";
	   defines[def_it].Definition	=	"1";
	   def_it++;
   }

   if (o.dx10_minmax_sm)
   {
	   defines[def_it].Name		=	"USE_MINMAX_SM";
	   defines[def_it].Definition	=	"1";
	   def_it++;
   }

	// add a #define for DX10_1 MSAA support
   if( o.dx10_msaa )
   {
	   static char samples[2];

	   defines[def_it].Name		=	"USE_MSAA";
	   defines[def_it].Definition	=	"1";
	   def_it						++;

	   defines[def_it].Name		=	"MSAA_SAMPLES";
	   samples[0] = char(o.dx10_msaa_samples) + '0';
	   samples[1] = 0;
	   defines[def_it].Definition	= samples;	
	   def_it						++;

	   if( o.dx10_msaa_opt )
	   {
		   defines[def_it].Name		=	"MSAA_OPTIMIZATION";
		   defines[def_it].Definition	=	"1";
		   def_it						++;
	   }

		switch(o.dx10_msaa_alphatest)
		{
		case MSAA_ATEST_DX10_0_ATOC:
			defines[def_it].Name		=	"MSAA_ALPHATEST_DX10_0_ATOC";
			defines[def_it].Definition	=	"1";
			def_it						++;
			break;
		case MSAA_ATEST_DX10_1_ATOC:
			defines[def_it].Name		=	"MSAA_ALPHATEST_DX10_1_ATOC";
			defines[def_it].Definition	=	"1";
			def_it						++;
			break;
		case MSAA_ATEST_DX10_1_NATIVE:
			defines[def_it].Name		=	"MSAA_ALPHATEST_DX10_1";
			defines[def_it].Definition	=	"1";
			def_it						++;
			break;
		}
	   /*
	   VERIFY( !o.dx10_msaa_alphatest || (o.dx10_msaa_alphatest&&o.dx10_msaa_opt) || (o.dx10_msaa_alphatest&&o.dx10_msaa_alphatest_atoc)
		   || (o.dx10_msaa_alphatest&&o.dx10_msaa_hybrid));

	   if( o.dx10_msaa_alphatest )
	   {
		   if (o.dx10_msaa_alphatest_atoc)
		   {
			   defines[def_it].Name		=	"MSAA_ALPHATEST_ATOC";
			   defines[def_it].Definition	=	"1";
			   def_it						++;
		   }
		   else
		   {
			   defines[def_it].Name		=	"MSAA_ALPHATEST";
			   defines[def_it].Definition	=	"1";
			   def_it						++;
		   }
	   }
	   */
   }

	// finish
	defines[def_it].Name			=	0;
	defines[def_it].Definition		=	0;
	def_it							++;

	// 
	if (0==xr_strcmp(pFunctionName,"main"))	
	{
		if ('v'==pTarget[0])
      {
         if( HW.pDevice1 == 0 )
            pTarget = D3D10GetVertexShaderProfile(HW.pDevice);	// vertex	"vs_4_0"; //
         else
            pTarget = "vs_4_1";	// pixel	"ps_4_0"; //
      }
		else if ('p'==pTarget[0])
      {
         if( HW.pDevice1 == 0 )
            pTarget = D3D10GetPixelShaderProfile(HW.pDevice);	// pixel	"ps_4_0"; //
         else
            pTarget = "ps_4_1";	// pixel	"ps_4_0"; //
      }
		else if ('g'==pTarget[0])		
      {
         if( HW.pDevice1 == 0 )
            pTarget = D3D10GetGeometryShaderProfile(HW.pDevice);	// geometry	"gs_4_0"; //
         else
            pTarget = "gs_4_1";	// pixel	"ps_4_0"; //
      }
	}

	ID3DInclude*					pInclude		= (ID3DInclude*)		_pInclude;
	ID3DBlob**						ppShader		= (ID3DBlob**)			_ppShader;
	ID3DBlob**						ppErrorMsgs		= (ID3DBlob**)			_ppErrorMsgs;
	//LPD3DXCONSTANTTABLE*            ppConstantTable	= (LPD3DXCONSTANTTABLE*)_ppConstantTable;
	
	//HRESULT		_result	= D3D10CompileShader(pSrcData,SrcDataLen,NULL, defines,pInclude,pFunctionName,pTarget,Flags,ppShader,ppErrorMsgs,ppConstantTable);

	HRESULT		_result	= D3DX10CompileFromMemory( 
		pSrcData, 
		SrcDataLen,
		"",//NULL, //LPCSTR pFileName,	//	NVPerfHUD bug workaround.
		defines, pInclude, pFunctionName,
		pTarget,
      Flags, 0, NULL, 
		ppShader,
		ppErrorMsgs,NULL
		);

	if (SUCCEEDED(_result) && o.disasm)
	{
		ID3DBlob*		code	= *ppShader;
		ID3DBlob*		disasm	= 0;
		D3D10DisassembleShader(code->GetBufferPointer(), code->GetBufferSize(), FALSE, 0, &disasm);
		//D3DXDisassembleShader		(LPDWORD(code->GetBufferPointer()), FALSE, 0, &disasm );
		string_path			dname;
		strconcat			(sizeof(dname),dname,"disasm\\",name,('v'==pTarget[0])?".vs":('p'==pTarget[0])?".ps":".gs" );
		IWriter*			W		= FS.w_open("$logs$",dname);
		W->w				(disasm->GetBufferPointer(),(u32)disasm->GetBufferSize());
		FS.w_close			(W);
		_RELEASE			(disasm);
	}
	return		_result;
}
