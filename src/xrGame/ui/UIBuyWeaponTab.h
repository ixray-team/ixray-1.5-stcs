#pragma once

#include "UITabControl.h"
#include "UIStatic.h"

class CUIXml;

class CUIBuyWeaponTab : public CUITabControl 
{
public:
				CUIBuyWeaponTab				();
	virtual		~CUIBuyWeaponTab			();

	virtual void Init						(CUIXml* xml, char* path);
	virtual void OnTabChange				(const shared_str& sCur, const shared_str& sPrev);
			void SetActiveState				(bool bState = true);
private:
	bool			m_bActiveState;
	shared_str		m_sStubId;
};
