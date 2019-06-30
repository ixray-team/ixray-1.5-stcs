////////////////////////////////////////////////////////////////////////////
//	Module 		: UILogsWnd.h
//	Created 	: 25.04.2008
//	Author		: Evgeniy Sokolov
//	Description : UI Logs (PDA) window class
////////////////////////////////////////////////////////////////////////////

#ifndef UI_PDA_LOGS_WND_H_INCLUDED
#define UI_PDA_LOGS_WND_H_INCLUDED

#include "UIWindow.h"
#include "UIWndCallback.h"

#include "../ai_space.h"
#include "../../xrServerEntities/alife_space.h"

class CUIStatic;
class CUIXml;
class CUIProgressBar;
class CUIFrameLineWnd;
class CUICharacterInfo;
class CUIScrollView;
class CUI3tButtonEx;
class CUICheckButton;
struct GAME_NEWS_DATA;

class CUILogsWnd : public CUIWindow, public CUIWndCallback
{
private:
	typedef CUIWindow	inherited;

	CUIFrameLineWnd*	m_background;
	CUIStatic*			m_center_background;

	CUIStatic*			m_center_caption;
	CUICharacterInfo*	m_actor_ch_info;

	CUICheckButton*		m_filter_news;
	CUICheckButton*		m_filter_talk;
	
	CUIStatic*			m_date_caption;
	CUIStatic*			m_date;

	CUIStatic*			m_period_caption;
	CUIStatic*			m_period;

	ALife::_TIME_ID		m_start_game_time;
	ALife::_TIME_ID		m_selected_period;

	CUI3tButtonEx*		m_prev_period;
	CUI3tButtonEx*		m_next_period;
	
	CUIScrollView*		m_list;
	u32					m_previous_time;
	bool				m_need_add;

public:
						CUILogsWnd			();
	virtual				~CUILogsWnd			();

	virtual void 		Show				( bool status );
	virtual void		Update				();
	virtual void		SendMessage			( CUIWindow* pWnd, s16 msg, void* pData );

			void		Init				();

	IC		void		UpdateNews			()	{ m_need_add = true; }

protected:
			void		ReLoadNews			();
			void		AddNewsItem			( GAME_NEWS_DATA& news_data );
	ALife::_TIME_ID		GetShiftPeriod		( ALife::_TIME_ID datetime, int shift_day );

			void __stdcall	UpdateChecks	( CUIWindow* w, void* d);
			void __stdcall	PrevPeriod		( CUIWindow* w, void* d);
			void __stdcall	NextPeriod		( CUIWindow* w, void* d);

/*
protected:
	void		add_faction			( CUIXml& xml, shared_str const& faction_id );
	void		clear_all_factions		();
	bool	__stdcall	SortingLessFunction		( CUIWindow* left, CUIWindow* right );
*/
}; // class CUILogsWnd

#endif // UI_PDA_LOGS_WND_H_INCLUDED
