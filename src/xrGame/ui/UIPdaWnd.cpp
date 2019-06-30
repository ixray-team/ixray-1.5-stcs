#include "stdafx.h"
#include "UIPdaWnd.h"
#include "../Pda.h"

#include "xrUIXmlParser.h"
#include "UIXmlInit.h"
#include "UIInventoryUtilities.h"

#include "../HUDManager.h"
#include "../level.h"
#include "../game_cl_base.h"

#include "UIStatic.h"
#include "UIFrameWindow.h"
#include "UITabControl.h"
#include "UIPdaContactsWnd.h"
#include "UIMapWnd.h"
//.#include "UIDiaryWnd.h"
#include "UIFrameLineWnd.h"
//.#include "UIEncyclopediaWnd.h"
//.#include "UIStalkersRankingWnd.h"
#include "UIActorInfo.h"
//.#include "UIEventsWnd.h"
#include "object_broker.h"
#include "UIMessagesWindow.h"
#include "UIMainIngameWnd.h"
#include "UITabButton.h"
#include "UIAnimatedStatic.h"

#include "UIHelper.h"
#include "UIHint.h"
#include "UITaskWnd.h"
#include "UIFactionWarWnd.h"
#include "UIRankingWnd.h"
#include "UILogsWnd.h"

#define PDA_XML		"pda.xml"

u32 g_pda_info_state = 0;

void RearrangeTabButtons(CUITabControl* pTab);//, xr_vector<Fvector2>& vec_sign_places);

CUIPdaWnd::CUIPdaWnd()
{
	pUITaskWnd       = NULL;
	pUIFactionWarWnd = NULL;
	pUIRankingWnd    = NULL;
	pUILogsWnd       = NULL;
	m_hint_wnd       = NULL;
	Init();
}

CUIPdaWnd::~CUIPdaWnd()
{
	delete_data( pUITaskWnd );
	delete_data( pUIFactionWarWnd );
	delete_data( pUIRankingWnd );
	delete_data( pUILogsWnd );
	delete_data( m_hint_wnd );
	delete_data( UINoice );
}

void CUIPdaWnd::Init()
{
	CUIXml					uiXml;
	uiXml.Load				(CONFIG_PATH, UI_PATH, PDA_XML);

	m_pActiveDialog			= NULL;
	m_sActiveSection		= "";

	CUIXmlInit::InitWindow	(uiXml, "main", 0, this);

	UIMainPdaFrame			= UIHelper::CreateStatic( uiXml, "background_static", this );
	m_caption				= UIHelper::CreateStatic( uiXml, "caption_static", this );
	m_caption_const._set	( m_caption->GetText() );

	m_anim_static			= xr_new<CUIAnimatedStatic>();
	AttachChild				(m_anim_static);
	m_anim_static->SetAutoDelete(true);
	CUIXmlInit::InitAnimatedStatic(uiXml, "anim_static", 0, m_anim_static);

	m_btn_close				= UIHelper::Create3tButtonEx( uiXml, "close_button", this );
	m_hint_wnd				= UIHelper::CreateHint( uiXml, "hint_wnd" );

	if ( IsGameTypeSingle() )
	{
		pUITaskWnd					= xr_new<CUITaskWnd>();
		pUITaskWnd->hint_wnd		= m_hint_wnd;
		pUITaskWnd->Init			();

		pUIFactionWarWnd				= xr_new<CUIFactionWarWnd>();
		pUIFactionWarWnd->hint_wnd		= m_hint_wnd;
		pUIFactionWarWnd->Init			();

		pUIRankingWnd					= xr_new<CUIRankingWnd>();
		pUIRankingWnd->Init				();

		pUILogsWnd						= xr_new<CUILogsWnd>();
		pUILogsWnd->Init				();

	}

	UITabControl					= xr_new<CUITabControl>();
	UITabControl->SetAutoDelete		(true);
	AttachChild						(UITabControl);
	CUIXmlInit::InitTabControl		(uiXml, "tab", 0, UITabControl);
	UITabControl->SetMessageTarget	(this);

/*
	if(GameID()!=eGameIDSingle)
	{
		UITabControl->GetButtonsVector()->at(0)->Enable(false);
		UITabControl->GetButtonsVector()->at(2)->Enable(false);
		UITabControl->GetButtonsVector()->at(3)->Enable(false);
		UITabControl->GetButtonsVector()->at(4)->Enable(false);
		UITabControl->GetButtonsVector()->at(5)->Enable(false);
		UITabControl->GetButtonsVector()->at(6)->Enable(false);
	}
*/

/*
	m_updatedSectionImage	= xr_new<CUIStatic>();
	xml_init.InitStatic		(uiXml, "updated_section_static", 0, m_updatedSectionImage);

	m_oldSectionImage		= xr_new<CUIStatic>();
	xml_init.InitStatic		(uiXml, "old_section_static", 0, m_oldSectionImage);
*/

	UINoice					= xr_new<CUIStatic>();
	UINoice->SetAutoDelete	( true );
	CUIXmlInit::InitStatic	( uiXml, "noice_static", 0, UINoice );

	RearrangeTabButtons		(UITabControl);//, m_sign_places_main);
}

void CUIPdaWnd::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
{
	switch ( msg )
	{
	case TAB_CHANGED:
		{
			if ( pWnd == UITabControl )
			{
				SetActiveSubdialog			(UITabControl->GetActiveId());
			}
			break;
		}
	case BUTTON_CLICKED:
		{
			if ( pWnd == m_btn_close )
			{
				HUD().GetUI()->StartStopMenu( this, true );
			}
			break;
		}
	default:
		{
			R_ASSERT						(m_pActiveDialog);
			m_pActiveDialog->SendMessage	(pWnd, msg, pData);
		}
	};
}

void CUIPdaWnd::Show()
{
	InventoryUtilities::SendInfoToActor	("ui_pda");
	inherited::Show						();
	
	if ( !m_pActiveDialog )
	{
		SetActiveSubdialog				("eptTasks");
	}
	m_pActiveDialog->Show				(true);
}

void CUIPdaWnd::Hide()
{
	inherited::Hide();
	InventoryUtilities::SendInfoToActor					("ui_pda_hide");
	HUD().GetUI()->UIMainIngameWnd->SetFlashIconState_	(CUIMainIngameWnd::efiPdaTask, false);
	m_pActiveDialog->Show				(false);
}

void CUIPdaWnd::Update()
{
	inherited::Update();
	m_pActiveDialog->Update();
}

void CUIPdaWnd::SetActiveSubdialog(const shared_str& section)
{
	if ( m_sActiveSection == section ) return;

	if ( m_pActiveDialog )
	{
		UIMainPdaFrame->DetachChild( m_pActiveDialog );
		m_pActiveDialog->Show( false );
	}

	if ( section == "eptTasks" )
	{
		m_pActiveDialog = pUITaskWnd;
	}
	else if ( section == "eptFractionWar" )
	{
		m_pActiveDialog = pUIFactionWarWnd;
	}
	else if ( section == "eptRanking" )
	{
		m_pActiveDialog = pUIRankingWnd;
	}
	else if ( section == "eptLogs" )
	{
		m_pActiveDialog = pUILogsWnd;
	}

	R_ASSERT						(m_pActiveDialog);
	UIMainPdaFrame->AttachChild		(m_pActiveDialog);
	m_pActiveDialog->Show			(true);

	if ( UITabControl->GetActiveId() != section )
	{
		UITabControl->SetActiveTab( section );
	}
	m_sActiveSection = section;
	SetActiveCaption();
}

void CUIPdaWnd::SetActiveCaption()
{
	TABS_VECTOR*	btn_vec		= UITabControl->GetButtonsVector();
	TABS_VECTOR::iterator it_b	= btn_vec->begin();
	TABS_VECTOR::iterator it_e	= btn_vec->end();
	for ( ; it_b != it_e; ++it_b )
	{
		if ( (*it_b)->m_btn_id._get() == m_sActiveSection._get() )
		{
			LPCSTR cur = (*it_b)->GetText();
			string256 buf;
			strconcat( sizeof(buf), buf, m_caption_const.c_str(), cur );
			SetCaption( buf );
			return;
		}
	}
}

void CUIPdaWnd::Show_SecondTaskWnd( bool status )
{
	if ( status )
	{
		SetActiveSubdialog( "eptTasks" );
	}
	pUITaskWnd->Show_SecondTasksWnd( status );
}

void CUIPdaWnd::Show_MapLegendWnd( bool status )
{
	if ( status )
	{
		SetActiveSubdialog( "eptTasks" );
	}
	pUITaskWnd->ShowMapLegend( status );
}

void CUIPdaWnd::Draw()
{
	inherited::Draw();
//.	DrawUpdatedSections();
	DrawHint();
	UINoice->Draw(); // over all
}

void CUIPdaWnd::DrawHint()
{
	if ( m_pActiveDialog == pUITaskWnd )
	{
		pUITaskWnd->DrawHint();
	}
	else if ( m_pActiveDialog == pUIFactionWarWnd )
	{
//		m_hint_wnd->Draw();
	}
	else if ( m_pActiveDialog == pUIRankingWnd )
	{

	}
	else if ( m_pActiveDialog == pUILogsWnd )
	{

	}
	m_hint_wnd->Draw();
}

void CUIPdaWnd::UpdatePda()
{
	if ( m_pActiveDialog == pUILogsWnd )
	{
		pUILogsWnd->UpdateNews();
	}
	if ( m_pActiveDialog == pUITaskWnd )
	{
		pUITaskWnd->ReloadTaskInfo();
	}
}

/*
void CUIPdaWnd::PdaContentsChanged	(pda_section::part type)
{
	bool b = true;

	if(type==pda_section::encyclopedia){
		UIEncyclopediaWnd->ReloadArticles	();
	}else
	if(type==pda_section::news){
		UIDiaryWnd->AddNews					();
		UIDiaryWnd->MarkNewsAsRead			(UIDiaryWnd->IsShown());
	}else
	if(type==pda_section::quests)
	{
		R_ASSERT2(0,"ahtung");
//.		UIEventsWnd->Reload					();
	}else
	if(type==pda_section::contacts){
		UIPdaContactsWnd->Reload		();
		b = false;
	}

	if(b){
		g_pda_info_state |= type;
		HUD().GetUI()->UIMainIngameWnd->SetFlashIconState_(CUIMainIngameWnd::efiPdaTask, true);
	}
}

void draw_sign		(CUIStatic* s, Fvector2& pos)
{
	s->SetWndPos		(pos);
	s->Draw				();
}

void CUIPdaWnd::DrawUpdatedSections				()
{

	m_updatedSectionImage->Update				();
	m_oldSectionImage->Update					();
	
	Fvector2									tab_pos;
	UITabControl->GetAbsolutePos				(tab_pos);

	Fvector2 pos;

	pos = m_sign_places_main[eptQuests];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::quests)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);


	pos = m_sign_places_main[eptMap];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::map)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);


	pos = m_sign_places_main[eptDiary];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::diary)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);

	pos = m_sign_places_main[eptContacts];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::contacts)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);

	pos = m_sign_places_main[eptRanking];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::ranking)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);

	pos = m_sign_places_main[eptActorStatistic];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::statistics)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);

	pos = m_sign_places_main[eptEncyclopedia];
	pos.add(tab_pos);
	if(g_pda_info_state&pda_section::encyclopedia)
		draw_sign								(m_updatedSectionImage, pos);
	else
		draw_sign								(m_oldSectionImage, pos);
}
*/

void CUIPdaWnd::Reset()
{
	inherited::ResetAll		();

	if ( pUITaskWnd )		pUITaskWnd->ResetAll();
	if ( pUIFactionWarWnd )	pUITaskWnd->ResetAll();
	if ( pUIRankingWnd )	pUITaskWnd->ResetAll();
	if ( pUILogsWnd )		pUITaskWnd->ResetAll();
}

void CUIPdaWnd::SetCaption( LPCSTR text )
{
	m_caption->SetText( text );
}

void RearrangeTabButtons(CUITabControl* pTab)//, xr_vector<Fvector2>& vec_sign_places)
{
	TABS_VECTOR *	btn_vec		= pTab->GetButtonsVector();
	TABS_VECTOR::iterator it	= btn_vec->begin();
	TABS_VECTOR::iterator it_e	= btn_vec->end();
//	vec_sign_places.clear		();
//	vec_sign_places.resize		(btn_vec->size());

	Fvector2					pos;
	pos.set						((*it)->GetWndPos());
	float						size_x;
//	Fvector2					sign_sz;
//	sign_sz.set					(9.0f+3.0f, 11.0f);
//	sign_sz.set					(0.0f, 11.0f);
//	u32 idx						= 0;
//	float	btn_text_len		= 0.0f;
//	CUIStatic* st				= NULL;

//	for ( ; it != it_e; ++it, ++idx )
	for ( ; it != it_e; ++it )
	{
		/*if(idx!=0)
		{
			st					= xr_new<CUIStatic>(); 
			st->SetAutoDelete	(true);
			pTab->AttachChild	(st);
			st->SetFont			((*it)->GetFont());
			st->SetTextColor	(color_rgba(90,90,90,255));
			st->SetText			("_u//u_");
			st->SetWndSize		((*it)->GetWndSize());
			st->AdjustWidthToText();
			st->SetWndPos		(pos);
			pos.x				+= st->GetWndSize().x;
		}*/

//		vec_sign_places[idx].set(pos);
//		vec_sign_places[idx].y	+= iFloor(((*it)->GetWndSize().y - sign_sz.y)/2.0f);
//		vec_sign_places[idx].y	= (float)iFloor(vec_sign_places[idx].y);
//		pos.x					+= sign_sz.x;

		(*it)->SetWndPos		(pos);
		(*it)->AdjustWidthToText();
		size_x					= (*it)->GetWndSize().x + 30.0f;
		(*it)->SetWidth			(size_x);
		pos.x					+= size_x - 6.0f;
	}
	
	pTab->SetWidth( pos.x + 5.0f );
	pos.x = pTab->GetWndPos().x - pos.x;
	pos.y = pTab->GetWndPos().y;
	pTab->SetWndPos( pos );
}
