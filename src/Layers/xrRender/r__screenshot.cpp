#include "stdafx.h"
//#include "../../xrEngine/xr_effgamma.h"
#include "xr_effgamma.h"
#include "dxRenderDeviceRender.h"
#include "../xrRender/tga.h"
#include "../../xrEngine/xrImage_Resampler.h"

#ifdef	USE_DX10
#include "d3dx10tex.h"
#endif	//	USE_DX10

#define	GAMESAVE_SIZE	128

IC u32 convert				(float c)	{
	u32 C=iFloor(c);
	if (C>255) C=255;
	return C;
}
IC void MouseRayFromPoint	( Fvector& direction, int x, int y, Fmatrix& m_CamMat )
{
	int halfwidth		= Device.dwWidth/2;
	int halfheight		= Device.dwHeight/2;

	Ivector2 point2;
	point2.set			(x-halfwidth, halfheight-y);

	float size_y		= VIEWPORT_NEAR * tanf( deg2rad(60.f) * 0.5f );
	float size_x		= size_y / (Device.fHeight_2/Device.fWidth_2);

	float r_pt			= float(point2.x) * size_x / (float) halfwidth;
	float u_pt			= float(point2.y) * size_y / (float) halfheight;

	direction.mul		( m_CamMat.k, VIEWPORT_NEAR );
	direction.mad		( direction, m_CamMat.j, u_pt );
	direction.mad		( direction, m_CamMat.i, r_pt );
	direction.normalize	();
}

#ifdef	USE_DX10
void CRender::Screenshot		(IRender_interface::ScreenshotMode mode, LPCSTR name)
{
	ID3D10Resource		*pSrcTexture;
	HW.pBaseRT->GetResource(&pSrcTexture);

	VERIFY(pSrcTexture);

	// Save
	switch (mode)	
	{
		case IRender_interface::SM_FOR_GAMESAVE:
			{
				ID3D10Texture2D		*pSrcSmallTexture;
	
				D3D10_TEXTURE2D_DESC desc;
				ZeroMemory( &desc, sizeof(desc) );
				desc.Width = GAMESAVE_SIZE;
				desc.Height = GAMESAVE_SIZE;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_BC1_UNORM;
				desc.SampleDesc.Count = 1;
				desc.Usage = D3D10_USAGE_DEFAULT;
				desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
				CHK_DX( HW.pDevice->CreateTexture2D( &desc, NULL, &pSrcSmallTexture ) );

				//	D3DX10_TEXTURE_LOAD_INFO *pLoadInfo

				CHK_DX(D3DX10LoadTextureFromTexture( pSrcTexture,
					NULL, pSrcSmallTexture ));

				// save (logical & physical)
				ID3DBlob*		saved	= 0;
				HRESULT hr					= D3DX10SaveTextureToMemory( pSrcSmallTexture, D3DX10_IFF_DDS, &saved, 0);
				//HRESULT hr					= D3DXSaveTextureToFileInMemory (&saved,D3DXIFF_DDS,texture,0);
				if(hr==D3D_OK)
				{
					IWriter*			fs		= FS.w_open	(name); 
					if (fs)				
					{
						fs->w				(saved->GetBufferPointer(),(u32)saved->GetBufferSize());
						FS.w_close			(fs);
					}
				}
				_RELEASE			(saved);

				// cleanup
				_RELEASE			(pSrcSmallTexture);
			}
			break;
		case IRender_interface::SM_NORMAL:
			{
				string64			t_stemp;
				string_path			buf;
				sprintf_s			(buf,sizeof(buf),"ss_%s_%s_(%s).jpg",Core.UserName,timestamp(t_stemp),(g_pGameLevel)?g_pGameLevel->name().c_str():"mainmenu");
				ID3DBlob			*saved	= 0;
				CHK_DX				(D3DX10SaveTextureToMemory( pSrcTexture, D3DX10_IFF_JPG, &saved, 0));
				IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
				fs->w				(saved->GetBufferPointer(),(u32)saved->GetBufferSize());
				FS.w_close			(fs);
				_RELEASE			(saved);

				if (strstr(Core.Params,"-ss_tga"))	
				{ // hq
					sprintf_s			(buf,sizeof(buf),"ssq_%s_%s_(%s).tga",Core.UserName,timestamp(t_stemp),(g_pGameLevel)?g_pGameLevel->name().c_str():"mainmenu");
					ID3DBlob*		saved	= 0;
					CHK_DX				(D3DX10SaveTextureToMemory( pSrcTexture, D3DX10_IFF_BMP, &saved, 0));
					//		CHK_DX				(D3DXSaveSurfaceToFileInMemory (&saved,D3DXIFF_TGA,pFB,0,0));
					IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
					fs->w				(saved->GetBufferPointer(),(u32)saved->GetBufferSize());
					FS.w_close			(fs);
					_RELEASE			(saved);
				}
			}
			break;
		case IRender_interface::SM_FOR_LEVELMAP:
		case IRender_interface::SM_FOR_CUBEMAP:
			{
				VERIFY(!"CRender::Screenshot. This screenshot type is not supported for DX10.");
				/*
				string64			t_stemp;
				string_path			buf;
				VERIFY				(name);
				strconcat			(sizeof(buf),buf,"ss_",Core.UserName,"_",timestamp(t_stemp),"_#",name);
				strcat				(buf,".tga");
				IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
				TGAdesc				p;
				p.format			= IMG_24B;

				//	TODO: DX10: This is totally incorrect but mimics 
				//	original behaviour. Fix later.
				hr					= pFB->LockRect(&D,0,D3DLOCK_NOSYSLOCK);
				if(hr!=D3D_OK)		return;
				hr					= pFB->UnlockRect();
				if(hr!=D3D_OK)		goto _end_;

				// save
				u32* data			= (u32*)xr_malloc(Device.dwHeight*Device.dwHeight*4);
				imf_Process			(data,Device.dwHeight,Device.dwHeight,(u32*)D.pBits,Device.dwWidth,Device.dwHeight,imf_lanczos3);
				p.scanlenght		= Device.dwHeight*4;
				p.width				= Device.dwHeight;
				p.height			= Device.dwHeight;
				p.data				= data;
				p.maketga			(*fs);
				xr_free				(data);

				FS.w_close			(fs);
				*/
			}
			break;
	}

	_RELEASE(pSrcTexture);
}

#else	//	USE_DX10

void CRender::Screenshot		(IRender_interface::ScreenshotMode mode, LPCSTR name)
{
	if (!Device.b_is_Ready)			return;
	if ((psDeviceFlags.test(rsFullscreen)) == 0) {
		Log("~ Can't capture screen while in windowed mode...");
		return;
	}

	// Create temp-surface
	IDirect3DSurface9*	pFB;
	D3DLOCKED_RECT		D;
	HRESULT				hr;
	hr					= HW.pDevice->CreateOffscreenPlainSurface(Device.dwWidth,Device.dwHeight,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,&pFB,NULL);
	if(hr!=D3D_OK)		return;

	hr					= HW.pDevice->GetFrontBufferData(0,pFB);
	if(hr!=D3D_OK)		return;

	
	hr					= pFB->LockRect(&D,0,D3DLOCK_NOSYSLOCK);
	if(hr!=D3D_OK)		return;

	// Image processing (gamma-correct)
	u32* pPixel		= (u32*)D.pBits;
	u32* pEnd		= pPixel+(Device.dwWidth*Device.dwHeight);
	//	IGOR: Remove inverse color correction and kill alpha
	/*
	D3DGAMMARAMP	G;
	dxRenderDeviceRender::Instance().gammaGenLUT(G);
	for (int i=0; i<256; i++) {
		G.red	[i]	/= 256;
		G.green	[i]	/= 256;
		G.blue	[i]	/= 256;
	}
	for (;pPixel!=pEnd; pPixel++)	{
		u32 p = *pPixel;
		*pPixel = color_xrgb	(
			G.red	[color_get_R(p)],
			G.green	[color_get_G(p)],
			G.blue	[color_get_B(p)]
			);
	}
	*/

	//	Kill alpha
	for (;pPixel!=pEnd; pPixel++)	
	{
		u32 p = *pPixel;
		*pPixel = color_xrgb	(
			color_get_R(p),
			color_get_G(p),
			color_get_B(p)
		);
	}

	hr					= pFB->UnlockRect();
	if(hr!=D3D_OK)		goto _end_;
	

	// Save
	switch (mode)	{
		case IRender_interface::SM_FOR_GAMESAVE:
			{
				// texture
				ID3DTexture2D*	texture	= NULL;
				hr					= D3DXCreateTexture(HW.pDevice,GAMESAVE_SIZE,GAMESAVE_SIZE,1,0,D3DFMT_DXT1,D3DPOOL_SCRATCH,&texture);
				if(hr!=D3D_OK)		goto _end_;
				if(NULL==texture)	goto _end_;

				// resize&convert to surface
				IDirect3DSurface9*	surface = 0;
				hr					= texture->GetSurfaceLevel(0,&surface);
				if(hr!=D3D_OK)		goto _end_;
				VERIFY				(surface);
				hr					= D3DXLoadSurfaceFromSurface(surface,0,0,pFB,0,0,D3DX_DEFAULT,0);
				_RELEASE			(surface);
				if(hr!=D3D_OK)		goto _end_;

				// save (logical & physical)
				ID3DBlob*		saved	= 0;
				hr					= D3DXSaveTextureToFileInMemory (&saved,D3DXIFF_DDS,texture,0);
				if(hr!=D3D_OK)		goto _end_;
				
				IWriter*			fs		= FS.w_open	(name); 
				if (fs)				{
					fs->w				(saved->GetBufferPointer(),saved->GetBufferSize());
					FS.w_close			(fs);
				}
				_RELEASE			(saved);

				// cleanup
				_RELEASE			(texture);
			}
			break;
		case IRender_interface::SM_NORMAL:
			{
				string64			t_stemp;
				string_path			buf;
				sprintf_s			(buf,sizeof(buf),"ss_%s_%s_(%s).jpg",Core.UserName,timestamp(t_stemp),(g_pGameLevel)?g_pGameLevel->name().c_str():"mainmenu");
				ID3DBlob*		saved	= 0;
				CHK_DX				(D3DXSaveSurfaceToFileInMemory (&saved,D3DXIFF_JPG,pFB,0,0));
				IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
				fs->w				(saved->GetBufferPointer(),saved->GetBufferSize());
				FS.w_close			(fs);
				_RELEASE			(saved);
				if (strstr(Core.Params,"-ss_tga"))	{ // hq
					sprintf_s			(buf,sizeof(buf),"ssq_%s_%s_(%s).tga",Core.UserName,timestamp(t_stemp),(g_pGameLevel)?g_pGameLevel->name().c_str():"mainmenu");
					ID3DBlob*		saved	= 0;
					CHK_DX				(D3DXSaveSurfaceToFileInMemory (&saved,D3DXIFF_TGA,pFB,0,0));
					IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
					fs->w				(saved->GetBufferPointer(),saved->GetBufferSize());
					FS.w_close			(fs);
					_RELEASE			(saved);
				}
			}
			break;
		case IRender_interface::SM_FOR_LEVELMAP:
		case IRender_interface::SM_FOR_CUBEMAP:
			{
//				string64			t_stemp;
				string_path			buf;
				VERIFY				(name);
				strconcat			(sizeof(buf), buf, name, ".tga");
				IWriter*		fs	= FS.w_open	("$screenshots$",buf); R_ASSERT(fs);
				TGAdesc				p;
				p.format			= IMG_24B;

				//	TODO: DX10: This is totally incorrect but mimics 
				//	original behaviour. Fix later.
				hr					= pFB->LockRect(&D,0,D3DLOCK_NOSYSLOCK);
				if(hr!=D3D_OK)		return;
				hr					= pFB->UnlockRect();
				if(hr!=D3D_OK)		goto _end_;

				// save
				u32* data			= (u32*)xr_malloc(Device.dwHeight*Device.dwHeight*4);
				imf_Process			(data,Device.dwHeight,Device.dwHeight,(u32*)D.pBits,Device.dwWidth,Device.dwHeight,imf_lanczos3);
				p.scanlenght		= Device.dwHeight*4;
				p.width				= Device.dwHeight;
				p.height			= Device.dwHeight;
				p.data				= data;
				p.maketga			(*fs);
				xr_free				(data);

				FS.w_close			(fs);
			}
			break;
	}

_end_:
	_RELEASE		(pFB);
}

#endif	//	USE_DX10