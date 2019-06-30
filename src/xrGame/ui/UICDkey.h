//
#pragma once
#include "UIEditBox.h"

class CUICDkey : public CUIEditBox
{
private:
	typedef			CUIEditBox		inherited;

public:
						CUICDkey		();
	virtual	void	SetText			(LPCSTR str) {}
	virtual	LPCSTR	GetText			();
	// CUIOptionsItem
	virtual void	SetCurrentValue	();
	virtual void	SaveValue		();
	virtual bool	IsChanged		();
	
			void	CreateCDKeyEntry();			

	virtual void	Draw();
	virtual void	OnFocusLost		();

private:
			LPCSTR	AddHyphens(LPCSTR str);
			LPCSTR	DelHyphens(LPCSTR str);

}; // class CUICDkey

class CUIMPPlayerName : public CUIEditBox
{
private:
	typedef			CUIEditBox		inherited;

public:
					CUIMPPlayerName	() {};
	virtual			~CUIMPPlayerName() {};

//	virtual	void	SetText			(LPCSTR str) {}

//	virtual void	SetCurrentValue();
//	virtual void	SaveValue();
//	virtual bool	IsChanged();

	virtual void	OnFocusLost		();

}; // class CUIMPPlayerName

extern	void	GetCDKey_FromRegistry		(char* cdkey);
extern	void	WriteCDKey_ToRegistry		(LPSTR cdkey);
extern	void	GetPlayerName_FromRegistry	(char* name);
extern	void	WritePlayerName_ToRegistry	(LPSTR name);
