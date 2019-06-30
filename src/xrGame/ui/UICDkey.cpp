
#include "stdafx.h"
#include "UICDkey.h"
#include "UILines.h"
#include "../../xrEngine/line_edit_control.h"

#include "UIColorAnimatorWrapper.h"
#include "../../xrEngine/xr_IOConsole.h"
#include "../RegistryFuncs.h"
#include "../../xrGameSpy/xrGameSpy_MainDefs.h"

extern string64	gsCDKey;

CUICDkey::CUICDkey()
{
	CreateCDKeyEntry();
	SetCurrentValue();
}

void CUICDkey::OnFocusLost()
{
	inherited::OnFocusLost();
	if(m_bInputFocus)
	{
		m_bInputFocus = false;
		GetMessageTarget()->SendMessage(this,EDIT_TEXT_COMMIT,NULL);
	}
	SaveValue();
}

void CUICDkey::Draw()
{
	//inherited::Draw();
	Frect						rect;
	GetAbsoluteRect				(rect);
	Fvector2					out;

	out.y						= (m_wndSize.y - m_pLines->m_pFont->CurrentHeight_())/2.0f;
	out.x						= 0.0f;
	m_pLines->m_pFont->SetColor	(m_pLines->GetTextColor());

	Fvector2					pos;
	pos.set						(rect.left+out.x, rect.top+out.y);
	UI()->ClientToScreenScaled	(pos);

	if ( m_bInputFocus )
	{		
		m_pLines->m_pFont->Out	( pos.x, pos.y, "%s", AddHyphens( ec().str_edit() ) );
		
		float _h				= m_pLines->m_pFont->CurrentHeight_();
		UI()->ClientToScreenScaledHeight(_h);
		
		out.y					= rect.top + (m_wndSize.y - _h)/2.0f;
		
		float	w_tmp			= 0.0f;
		int i					= (int)xr_strlen( ec().str_before_cursor() );
		w_tmp					= m_pLines->m_pFont->SizeOf_( ec().str_before_cursor() );
		UI()->ClientToScreenScaledWidth( w_tmp );
		out.x					= rect.left + w_tmp;
		
		w_tmp					= m_pLines->m_pFont->SizeOf_("-");
		UI()->ClientToScreenScaledWidth( w_tmp );
		
		if(i>3)
			out.x	+= w_tmp;
		if(i>7)
			out.x	+= w_tmp;
		if(i>11)
			out.x	+= w_tmp;

		UI()->ClientToScreenScaled	(out);
		m_pLines->m_pFont->Out		(out.x, out.y, "_");
	}
	else
	{
		string64 tmp = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		u32 sz = xr_strlen( ec().str_edit() );
		tmp[sz] = 0;

		m_pLines->m_pFont->Out(pos.x, pos.y, "%s" ,AddHyphens(tmp) );
	}
	m_pLines->m_pFont->OnRender();
}

LPCSTR CUICDkey::GetText()
{
	return AddHyphens(inherited::GetText());
}

LPCSTR CUICDkey::AddHyphens( LPCSTR c )
{
	static string32 buf;

	int sz = xr_strlen(c);
	int j = 0; 

	for (int i = 1; i<=3; i++)
		buf[i*5 - 1]='-';

	for (int i = 0; i<sz; i++)
	{
		j = i + iFloor(i/4.0f);
		buf[j] = c[i];		
	}
	buf[sz + iFloor(sz/4.0f)] = 0;

	return buf;
}

LPCSTR CUICDkey::DelHyphens( LPCSTR c )
{
	static string32 buf;

	int sz = xr_strlen(c);
	int j = 0; 

	for (int i = 0; i<sz - _min(iFloor(sz/4.0f),3); i++)
	{
		j = i + iFloor(i/4.0f);
		buf[i] = c[j];		
	}
	buf[sz - _min(iFloor(sz/4.0f),3)] = 0;

	return buf;
}

void CUICDkey::SetCurrentValue()
{
	string512	CDKeyStr;
	CDKeyStr[0] = 0;
	GetCDKey_FromRegistry(CDKeyStr);
	inherited::SetText( DelHyphens(CDKeyStr) );
}

void CUICDkey::SaveValue()
{
	CUIOptionsItem::SaveValue();

//	char NewCDKey[32];
//	HKEY KeyCDKey = 0;

//	string256 tmp;
	strcpy_s( gsCDKey, sizeof(gsCDKey), AddHyphens(inherited::GetText()) );
	WriteCDKey_ToRegistry( gsCDKey );
//	sprintf_s(gsCDKey,"%s",AddHyphens(inherited::GetText()));
//	sprintf_s(tmp,"cdkey %s",AddHyphens(m_lines.GetText()));
//	Console->Execute(tmp);
}

bool CUICDkey::IsChanged()
{
	string512	tmpCDKeyStr;
	GetCDKey_FromRegistry(tmpCDKeyStr);
	return 0 != xr_strcmp(tmpCDKeyStr, inherited::GetText());
}

void CUICDkey::CreateCDKeyEntry()
{
}

 //=================================================================

void CUIMPPlayerName::OnFocusLost()
{
	inherited::OnFocusLost();
	if ( m_bInputFocus )
	{
		m_bInputFocus = false;
		GetMessageTarget()->SendMessage(this, EDIT_TEXT_COMMIT, NULL);
	}
	string64 name;
	strcpy_s( name, GetText() );
	WritePlayerName_ToRegistry( name );
}

// -------------------------------------------------------------------------------------------------

void GetCDKey_FromRegistry(char* cdkey)
{
	ReadRegistry_StrValue(REGISTRY_VALUE_GSCDKEY, cdkey);
	if ( xr_strlen(cdkey) > 64 )
	{
		cdkey[64] = 0;
	}
}

void WriteCDKey_ToRegistry(LPSTR cdkey)
{
	if ( xr_strlen(cdkey) > 64 )
	{
		cdkey[64] = 0;
	}
	WriteRegistry_StrValue(REGISTRY_VALUE_GSCDKEY, cdkey);
}

void GetPlayerName_FromRegistry(char* name)
{
	ReadRegistry_StrValue(REGISTRY_VALUE_USERNAME, name);
	if ( xr_strlen(name) > 17 )
	{
		name[17] = 0;
	}
}

void WritePlayerName_ToRegistry(LPSTR name)
{
	if ( xr_strlen(name) > 17 )
	{
		name[17] = 0;
	}
	WriteRegistry_StrValue(REGISTRY_VALUE_USERNAME, name);
}
