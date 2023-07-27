#include "stdafx.h"
#include "IXRayGameConstants.h"
#include "GamePersistent.h"

bool	m_bUseHighQualityIcons = false;
bool	m_bUseLoadScreenByLang = false;

namespace GameConstants
{
	void LoadConstants()
	{
		m_bUseHighQualityIcons = READ_IF_EXISTS(pSettings, r_bool, "inventory", "use_hq_icons", false);
		m_bUseLoadScreenByLang = READ_IF_EXISTS(pSettings, r_bool, "ui_settings", "use_loadscreen_by_lang", false);

		Msg("# IX-Ray GameConstants are loaded");
	}
	
	bool GetUseHQ_Icons()
	{
		return m_bUseHighQualityIcons;
	}
	
	bool GetUseLoadScreenByLang()
	{
		return m_bUseLoadScreenByLang;
	}
}