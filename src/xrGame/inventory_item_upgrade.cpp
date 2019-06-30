////////////////////////////////////////////////////////////////////////////
//	Module 		: inventory_item_upgrade.cpp
//	Created 	: 27.11.2007
//  Modified 	: 27.11.2007
//	Author		: Sokolov Evgeniy
//	Description : Inventory item upgrades class impl
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "inventory_item.h"
#include "inventory_item_impl.h"

#include "ai_object_location.h"
#include "object_broker.h"

#include "ai_space.h"
#include "alife_simulator.h"
#include "inventory_upgrade_manager.h"
#include "inventory_upgrade.h"


bool CInventoryItem::has_upgrade( const shared_str& upgrade_id )
{
	if ( m_section_id == upgrade_id )
	{
		return true;
	}
	return ( std::find( m_upgrades.begin(), m_upgrades.end(), upgrade_id ) != m_upgrades.end() );
}

void CInventoryItem::add_upgrade( const shared_str& upgrade_id )
{
	if ( !has_upgrade( upgrade_id ) )
	{
		m_upgrades.push_back( upgrade_id );
	}
}

bool CInventoryItem::get_upgrades_str( string2048& res ) const
{
	int prop_count = 0;
	res[0] = 0;
	Upgrades_type::const_iterator ib = m_upgrades.begin();
	Upgrades_type::const_iterator ie = m_upgrades.end();
	inventory::upgrade::Upgrade* upgr;
	for ( ; ib != ie; ++ib )
	{
		upgr = ai().alife().inventory_upgrade_manager().get_upgrade( *ib );
		if ( !upgr ) { continue; }

		LPCSTR upgr_section = upgr->section();
		if ( prop_count > 0 )
		{
			strcat_s( res, sizeof(res), ", " );
		}
		strcat_s( res, sizeof(res), upgr_section );
		++prop_count;
	}
	if ( prop_count > 0 )
	{
		return true;
	}
	return false;
}

bool CInventoryItem::equal_upgrades( Upgrades_type const& other_upgrades ) const
{
	if ( m_upgrades.size() != other_upgrades.size() )
	{
		return false;
	}
	
	Upgrades_type::const_iterator ib = m_upgrades.begin();
	Upgrades_type::const_iterator ie = m_upgrades.end();
	for ( ; ib != ie; ++ib )
	{
		shared_str const& name1 = (*ib);
		bool upg_equal = false;
		Upgrades_type::const_iterator ib2 = other_upgrades.begin();
		Upgrades_type::const_iterator ie2 = other_upgrades.end();
		for ( ; ib2 != ie2; ++ib2 )
		{
			if ( name1.equal( (*ib2) ) )
			{
				upg_equal = true;
				break;//from for2, in for1
			}
		}
		if ( !upg_equal )
		{
			return false;
		}
	}
	return true;
}

#ifdef DEBUG	
void CInventoryItem::log_upgrades()
{
	Msg( "* all upgrades of item = %s", m_section_id.c_str() );
	Upgrades_type::const_iterator ib = m_upgrades.begin();
	Upgrades_type::const_iterator ie = m_upgrades.end();
	for ( ; ib != ie; ++ib )
	{
		Msg( "    %s", (*ib).c_str() );
	}
	Msg( "* finish - upgrades of item = %s", m_section_id.c_str() );
}
#endif // DEBUG

void CInventoryItem::init_upgrades()
{
	if( ai().get_alife() )
	{
		ai().alife().inventory_upgrade_manager().init_install( *this );
	}
}

void CInventoryItem::install_loaded_upgrades() // after load
{
	size_t const count			= m_upgrades.size();
	typedef buffer_vector<shared_str>	Upgrades;
	Upgrades					copy_upgrades(
		_alloca(sizeof(shared_str) * count),
		count,
		m_upgrades.begin(),
		m_upgrades.end()
	);
	m_upgrades.clear_not_free();

	Upgrades::iterator ib = copy_upgrades.begin();
	Upgrades::iterator ie = copy_upgrades.end();
	for ( ; ib != ie ; ++ib )
	{
		ai().alife().inventory_upgrade_manager().upgrade_install( *this, (*ib), true );
	}
}

bool CInventoryItem::install_upgrade( LPCSTR section )
{
	return install_upgrade_impl( section, false );
}

bool CInventoryItem::verify_upgrade( LPCSTR section )
{
	return install_upgrade_impl( section, true );
}

bool CInventoryItem::install_upgrade_impl( LPCSTR section, bool test )
{
	bool result = process_if_exists( section, "cost",   &CInifile::r_u32,   m_cost,   test );
	result |= process_if_exists( section, "inv_weight", &CInifile::r_float, m_weight, test );

	bool result2 = false;
	if ( m_slot != -1 )
	{
		BOOL value = m_flags.test( FRuckDefault );
		result2 = process_if_exists_set( section, "default_to_ruck", &CInifile::r_bool, value, test );
		if ( result2 && !test )
		{
			m_flags.set( FRuckDefault, value );
		}
		result |= result2;

		value = m_flags.test( FAllowSprint );
		result2 = process_if_exists_set( section, "sprint_allowed", &CInifile::r_bool, value, test );
		if ( result2 && !test )
		{
			m_flags.set( FAllowSprint, value );
		}
		result |= result2;

		result |= process_if_exists( section, "control_inertion_factor", &CInifile::r_float, m_fControlInertionFactor, test );
	}

	LPCSTR str;
	result2 = process_if_exists_set( section, "immunities_sect", &CInifile::r_string, str, test );
	if ( result2 && !test )
	{
		CHitImmunity::LoadImmunities( str, pSettings );
	}
	return result;
}
