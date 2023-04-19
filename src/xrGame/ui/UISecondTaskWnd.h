////////////////////////////////////////////////////////////////////////////
//	Module 		: UISecondTaskWnd.h
//	Created 	: 30.05.2008
//	Author		: Evgeniy Sokolov
//	Description : UI Secondary Task Wnd class
////////////////////////////////////////////////////////////////////////////

#ifndef UI_SECOND_TASK_WND_H_INCLUDED
#define UI_SECOND_TASK_WND_H_INCLUDED

#include "UIWindow.h"
#include "UIWndCallback.h"

#define PDA_TASK_XML	"pda_tasks.xml"

class CUIXml;
class CUIFrameWindow;
class CUIScrollView;
class CUIStatic;
class CUI3tButton;
class CUI3tButtonEx;
class CUICheckButton;
class CUIFrameLineWnd;
class CGameTask;
class UIHint;

class UISecondTaskWnd : public CUIWindow, public CUIWndCallback
{
private:
	typedef CUIWindow	inherited;

public:
					UISecondTaskWnd		();
	virtual			~UISecondTaskWnd	();

			void	init_from_xml		( CUIXml& xml, LPCSTR path );

	virtual bool	OnMouseAction				( float x, float y, EUIMessages mouse_action );
	virtual void	Show				( bool status );
	virtual void 	OnFocusReceive		();
	virtual void	OnFocusLost			();
	virtual void	Update				();
	virtual void	SendMessage			( CUIWindow* pWnd, s16 msg, void* pData );

			void	UpdateList			();

protected:
	void xr_stdcall	OnBtnClose			( CUIWindow* w, void* d);
	bool xr_stdcall	SortingLessFunction	( CUIWindow* left, CUIWindow* right );

//			void	UpdateCounter		();
public:
	UIHint*				hint_wnd;

private: // m_
	CUIFrameWindow*		m_background;
	CUIScrollView*		m_list;
	
	CUIStatic*			m_caption;
//	CUIStatic*			m_counter;
	CUI3tButtonEx*		m_bt_close;

	u32					m_activ_task_count;
	float				m_orig_h;

}; // class UISecondTaskWnd

// -------------------------------------------------------------------------------------------------

class UISecondTaskItem : public CUIWindow
{
private:
	typedef CUIWindow	inherited;

public:
					UISecondTaskItem	();
	virtual			~UISecondTaskItem	();

			bool	init_task			( CGameTask* task, UISecondTaskWnd* parent );
	IC		u32		get_priority_task	() const;

	virtual void 	OnFocusReceive		();
	virtual void	OnFocusLost			();
	virtual void	Update				();
	virtual void	SendMessage			( CUIWindow* pWnd, s16 msg, void* pData );
	virtual bool	OnMouseAction				( float x, float y, EUIMessages mouse_action );

private:
			void	hide_hint			();
			void	update_view			();
			void	update_visible_map_spot();

public:
			bool	show_hint_can;
			bool	show_hint;

private: // m_
	CGameTask*		m_task;
	CUI3tButtonEx*	m_name;
	CUICheckButton*	m_bt_view;
	CUI3tButtonEx*	m_bt_focus;

	enum
	{
		stt_activ = 0,
		stt_unread,
		stt_read,
		stt_count
	};
	u32				m_color_states[stt_count];

}; // class UISecondTaskItem

#endif // UI_SECOND_TASK_WND_H_INCLUDED
