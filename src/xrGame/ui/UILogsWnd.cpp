////////////////////////////////////////////////////////////////////////////
//	Module 		: UILogsWnd.cpp
//	Created 	: 25.04.2008
//	Author		: Evgeniy Sokolov
//	Description : UI Logs (PDA) window class implementation
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UILogsWnd.h"

#include "UIXmlInit.h"
#include "UIProgressBar.h"
#include "UIFrameLineWnd.h"
#include "UIScrollView.h"
#include "UICheckButton.h"
#include "UIHelper.h"
#include "UICharacterInfo.h"
#include "UIInventoryUtilities.h"

#include "../actor.h"
#include "../game_news.h"
#include "../alife_time_manager.h"
#include "../alife_registry_wrappers.h"
#include "../string_table.h"
#include "UINewsItemWnd.h"


#define  PDA_LOGS_XML		"pda_logs.xml"

extern u64	generate_time	(u32 years, u32 months, u32 days, u32 hours, u32 minutes, u32 seconds, u32 milliseconds = 0);
extern void	split_time		(u64 time, u32 &years, u32 &months, u32 &days, u32 &hours, u32 &minutes, u32 &seconds, u32 &milliseconds);

u64 const day2ms = u64( 24 * 60 * 60 * 1000 );

CUILogsWnd::CUILogsWnd()
{
	m_actor_ch_info = NULL;
	m_previous_time = Device.dwTimeGlobal;
	m_selected_period = 0;
}

CUILogsWnd::~CUILogsWnd()
{
}


void CUILogsWnd::Show( bool status )
{
	if ( status )
	{
		m_actor_ch_info->InitCharacter( Actor()->object_id() );
		m_need_add = true;
		m_selected_period = GetShiftPeriod( Level().GetGameTime(), 0 );
		Update();
	}
	//InventoryUtilities::SendInfoToActor("ui_pda_news_hide");
	inherited::Show( status );
}

void CUILogsWnd::Update()
{
	inherited::Update();
	if ( IsShown() )
	{
		if ( Device.dwTimeGlobal - m_previous_time > 1000 )
		{
			m_previous_time = Device.dwTimeGlobal;
			m_date->SetText( InventoryUtilities::Get_GameTimeAndDate_AsString().c_str() );

			m_date_caption->AdjustWidthToText();
			Fvector2 pos = m_date_caption->GetWndPos();
			pos.x = m_date->GetWndPos().x - m_date_caption->GetWidth() - 5.0f;
			m_date_caption->SetWndPos( pos );
		}
	}
	if ( m_need_add )
	{
		ReLoadNews();
	}
}

void CUILogsWnd::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
{
	inherited::SendMessage( pWnd, msg, pData );
	CUIWndCallback::OnEvent( pWnd, msg, pData );
}

void CUILogsWnd::Init()
{
//	Fvector2 pos;
	CUIXml xml;
	xml.Load( CONFIG_PATH, UI_PATH, PDA_LOGS_XML );

	CUIXmlInit::InitWindow( xml, "main_wnd", 0, this );

	m_background		= UIHelper::CreateFrameLine( xml, "background", this );
	
	m_actor_ch_info = xr_new<CUICharacterInfo>();
	m_actor_ch_info->SetAutoDelete( true );
	AttachChild( m_actor_ch_info );
	m_actor_ch_info->InitCharacterInfo( &xml, "actor_ch_info" );

	m_center_background	= UIHelper::CreateStatic( xml, "center_background", this );
	m_center_caption	= UIHelper::CreateStatic( xml, "center_caption", this );

	string256 buf;
	strcpy_s( buf, sizeof(buf), m_center_caption->GetText() );
	strcat_s( buf, sizeof(buf), CStringTable().translate("ui_logs_center_caption").c_str() );
	m_center_caption->SetText( buf );

	m_list = xr_new<CUIScrollView>();
	m_list->SetAutoDelete( true );
	AttachChild( m_list );
	CUIXmlInit::InitScrollView( xml, "logs_list", 0, m_list );
//	m_list->SetWindowName("---logs_list");
//	m_logs_list->m_sort_function = fastdelegate::MakeDelegate( this, &CUIRankingWnd::SortingLessFunction );

	m_filter_news = UIHelper::CreateCheck( xml, "filter_news", this );
	m_filter_talk = UIHelper::CreateCheck( xml, "filter_talk", this );
	m_filter_news->SetCheck( true );
	m_filter_talk->SetCheck( true );

	m_date_caption = UIHelper::CreateStatic( xml, "date_caption", this );
	m_date         = UIHelper::CreateStatic( xml, "date", this );

	m_period_caption = UIHelper::CreateStatic( xml, "period_caption", this );
	m_period         = UIHelper::CreateStatic( xml, "period", this );

	m_prev_period = UIHelper::Create3tButtonEx( xml, "btn_prev_period", this );
	m_next_period = UIHelper::Create3tButtonEx( xml, "btn_next_period", this );

	Register( m_filter_news );
	Register( m_filter_talk );
	Register( m_prev_period );
	Register( m_next_period );

	AddCallback( m_filter_news->WindowName(), BUTTON_CLICKED, CUIWndCallback::void_function( this, &CUILogsWnd::UpdateChecks ) );
	AddCallback( m_filter_talk->WindowName(), BUTTON_CLICKED, CUIWndCallback::void_function( this, &CUILogsWnd::UpdateChecks ) );
	AddCallback( m_prev_period->WindowName(), BUTTON_CLICKED, CUIWndCallback::void_function( this, &CUILogsWnd::PrevPeriod ) );
	AddCallback( m_next_period->WindowName(), BUTTON_CLICKED, CUIWndCallback::void_function( this, &CUILogsWnd::NextPeriod ) );

	m_start_game_time = Level().GetStartGameTime();
	m_start_game_time = GetShiftPeriod( m_start_game_time, 0 );
}

void CUILogsWnd::ReLoadNews()
{
	m_list->Clear();

	if ( !Actor() )
	{
		m_need_add = false;
		return;
	}

	LPCSTR date_str = InventoryUtilities::GetDateAsString( m_selected_period, InventoryUtilities::edpDateToDay ).c_str();
	m_period->SetText( date_str );
	Fvector2 pos = m_period_caption->GetWndPos();
	pos.x = m_period->GetWndPos().x - m_period_caption->GetWidth() - m_prev_period->GetWidth() - 5.0f;
	m_period_caption->SetWndPos( pos );

	ALife::_TIME_ID end_period = GetShiftPeriod( m_selected_period, 1 );

	VERIFY( m_filter_news && m_filter_talk );
	GAME_NEWS_VECTOR& news_vector = Actor()->game_news_registry->registry().objects();

	// Показать только NEWS_TO_SHOW = 50 последних
	int currentNews = 0;

	GAME_NEWS_VECTOR::reverse_iterator ib = news_vector.rbegin();
	GAME_NEWS_VECTOR::reverse_iterator ie =	news_vector.rend();
	for ( ; ib != ie /*&& currentNews < NEWS_TO_SHOW*/ ; ++ib )
	{
		bool add = false;
		GAME_NEWS_DATA& gn = (*ib);
		if ( gn.m_type == GAME_NEWS_DATA::eNews && m_filter_news->GetCheck() )
		{
			add = true;
		}
		if ( gn.m_type == GAME_NEWS_DATA::eTalk && m_filter_talk->GetCheck() )
		{
			add = true;
		}
		if ( gn.receive_time < m_selected_period || end_period < gn.receive_time )
		{
			add = false;
		}

		if ( add )
		{
			AddNewsItem( gn );
			++currentNews;
		}
	}
	m_need_add = false;
}

void CUILogsWnd::AddNewsItem( GAME_NEWS_DATA& news_data )
{
	if ( news_data.m_type != GAME_NEWS_DATA::eNews &&
		 news_data.m_type != GAME_NEWS_DATA::eTalk )
	{
		return;
	}

	CUINewsItemWnd* news_itm = xr_new<CUINewsItemWnd>();
	news_itm->Init( PDA_LOGS_XML, "logs_item" );//_itm->Init(NEWS_XML,"talk_item")
	news_itm->Setup( news_data );
	
	m_list->AddWindow( news_itm, true );
}

void CUILogsWnd::UpdateChecks( CUIWindow* w, void* d )
{
	m_need_add = true;
}

void CUILogsWnd::PrevPeriod( CUIWindow* w, void* d )
{
	m_selected_period = GetShiftPeriod( m_selected_period, -1 );
	if ( m_selected_period < m_start_game_time )
	{
		m_selected_period = m_start_game_time;
	}
	m_need_add = true;
}

void CUILogsWnd::NextPeriod( CUIWindow* w, void* d )
{
	m_selected_period = GetShiftPeriod( m_selected_period, 1 ); // +1
	ALife::_TIME_ID game_time = GetShiftPeriod( Level().GetGameTime(), 0 );
	if ( m_selected_period > game_time  )
	{
		m_selected_period = game_time;
	}
	m_need_add = true;
}

ALife::_TIME_ID CUILogsWnd::GetShiftPeriod( ALife::_TIME_ID datetime, int shift_day )
{
	datetime -= (datetime % day2ms);
	datetime += (u64)shift_day * day2ms;
	return datetime;
}