#pragma once

#include "UIDialogWnd.h"
#include "../encyclopedia_article_defs.h"

class CInventoryOwner;
class CUIFrameLineWnd;
class CUIButton;
class CUI3tButtonEx;
class CUITabControl;
class CUIStatic;
class CUIXml;
class CUIFrameWindow;
class UIHint;

//.class CUIMapWnd;
//.class CUIEncyclopediaWnd;
//.class CUIDiaryWnd;
class CUIActorInfoWnd;
//.class CUIStalkersRankingWnd;
//.class CUIEventsWnd;
class CUIPdaContactsWnd;
class CUITaskWnd;
class CUIFactionWarWnd;
class CUIRankingWnd;
class CUILogsWnd;
class CUIAnimatedStatic;
class UIHint;


class CUIPdaWnd: public CUIDialogWnd
{
	typedef CUIDialogWnd	inherited;
protected:
	CUITabControl*			UITabControl;
	CUI3tButtonEx*			m_btn_close;

	CUIStatic*				UIMainPdaFrame;
	CUIStatic*				UINoice;
	
	CUIStatic*				m_caption;
	shared_str				m_caption_const;
	CUIAnimatedStatic*		m_anim_static;

	// Текущий активный диалог
	CUIWindow*				m_pActiveDialog;
	shared_str				m_sActiveSection;
//	xr_vector<Fvector2>		m_sign_places_main;

	UIHint*					m_hint_wnd;

public:
	CUITaskWnd*				pUITaskWnd;
	CUIFactionWarWnd*		pUIFactionWarWnd;
	CUIRankingWnd*			pUIRankingWnd;
	CUILogsWnd*				pUILogsWnd;

//.	CUIFrameLineWnd*		UIMainButtonsBackground;
//.	CUIFrameLineWnd*		UITimerBackground;
//.	CUIStatic*				m_updatedSectionImage;
//.	CUIStatic*				m_oldSectionImage;

//.	CUIMapWnd*				UIMapWnd;
//.	CUIPdaContactsWnd*		UIPdaContactsWnd;
//.	CUIEncyclopediaWnd*		UIEncyclopediaWnd;
//.	CUIDiaryWnd*			UIDiaryWnd;
//.	CUIActorInfoWnd*		UIActorInfo;
//.	CUIStalkersRankingWnd*	UIStalkersRanking;
//.	CUIEventsWnd*			UIEventsWnd;

//.	void					UpdateDateTime					();
//.	void					DrawUpdatedSections				();
	
	virtual void			Reset				();

public:
							CUIPdaWnd			();
	virtual					~CUIPdaWnd			();

	virtual void 			Init				();

	virtual void 			SendMessage			(CUIWindow* pWnd, s16 msg, void* pData = NULL);

	virtual void 			Draw				();
	virtual void 			Update				();
	virtual void 			Show				();
	virtual void 			Hide				();
	virtual bool			OnMouse				(float x, float y, EUIMessages mouse_action) {CUIDialogWnd::OnMouse(x,y,mouse_action);return true;} //always true because StopAnyMove() == false
		
			UIHint*			get_hint_wnd		() const { return m_hint_wnd; }
			void			DrawHint			();

			void			SetActiveCaption	();
			void			SetCaption			(LPCSTR text);
			void			Show_SecondTaskWnd	(bool status);
			void			Show_MapLegendWnd	(bool status);

			void			SetActiveSubdialog	(const shared_str& section);
	virtual bool			StopAnyMove			(){return false;}

			void			UpdatePda			();
//.			void			PdaContentsChanged	(pda_section::part type);
};
