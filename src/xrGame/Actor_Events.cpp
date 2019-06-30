#include "stdafx.h"
#include "actor.h"
#include "customdetector.h"
//#include "uigamesp.h"
//#include "hudmanager.h"
#include "weapon.h"
#include "artefact.h"
#include "scope.h"
#include "silencer.h"
#include "grenadelauncher.h"
#include "inventory.h"
#include "level.h"
#include "xr_level_controller.h"
#include "FoodItem.h"
#include "ActorCondition.h"
#include "Grenade.h"

#include "CameraLook.h"
#include "CameraFirstEye.h"
#include "holder_custom.h"
//.#include "ui/uiinventoryWnd.h"
#include "game_base_space.h"
#ifdef DEBUG
#include "PHDebug.h"
#endif

void CActor::OnEvent(NET_Packet& P, u16 type)
{
	inherited::OnEvent			(P,type);
	CInventoryOwner::OnEvent	(P,type);

	u16 id;
	switch (type)
	{
	case GE_TRADE_BUY:
	case GE_OWNERSHIP_TAKE:
		{
			P.r_u16					(id);
			CObject* O				= Level().Objects.net_Find	(id);
			R_ASSERT				(O);
		
			CGameObject* _GO		= smart_cast<CGameObject*>(O);
			
			if( inventory().CanTakeItem(smart_cast<CInventoryItem*>(_GO)) )
			{

				O->H_SetParent		(smart_cast<CObject*>(this));

				inventory().Take	(_GO, false, true);
			
				SelectBestWeapon(O);
			}else 
			{
				NET_Packet		P;
				u_EventGen		(P,GE_OWNERSHIP_REJECT,ID());
				P.w_u16			(u16(O->ID()));
				u_EventSend		(P);
			}
		}
		break;
	case GE_TRADE_SELL:
	case GE_OWNERSHIP_REJECT:
		{
			P.r_u16							(id);
			CObject* O						= Level().Objects.net_Find	(id);
			R_ASSERT2						(O, make_string("GE_OWNERSHIP_REJECT: Object not found, id = %d", id) );

			bool just_before_destroy		= !P.r_eof() && P.r_u8();
			O->SetTmpPreDestroy				(just_before_destroy);
			CGameObject * GO = smart_cast<CGameObject*>(O);
			VERIFY(GO);
			if (!O->getDestroy() && inventory().DropItem(GO, just_before_destroy)) 
			{
				//O->H_SetParent(0,just_before_destroy);//moved to DropItem
				//feel_touch_deny(O,2000);
				Level().m_feel_deny.feel_touch_deny(O, 1000);

				// [12.11.07] Alexander Maniluk: extended GE_OWNERSHIP_REJECT packet for drop item to selected position
				Fvector dropPosition;
				if (!P.r_eof())
				{
					
					P.r_vec3(dropPosition);
					GO->MoveTo(dropPosition);
					//Other variant :)
					/*NET_Packet MovePacket;
					MovePacket.w_begin(M_MOVE_ARTEFACTS);
					MovePacket.w_u8(1);
					MovePacket.w_u16(id);
					MovePacket.w_vec3(dropPosition);
					u_EventSend(MovePacket);*/
				}
			}

			if (!just_before_destroy)
				SelectBestWeapon(O);

		}
		break;
	case GE_INV_ACTION:
		{
			s32 cmd;
			P.r_s32		(cmd);
			u32 flags;
			P.r_u32		(flags);
			s32 ZoomRndSeed = P.r_s32();
			s32 ShotRndSeed = P.r_s32();
									
			if (flags & CMD_START)
			{
				if (cmd == kWPN_ZOOM)
					SetZoomRndSeed(ZoomRndSeed);
				if (cmd == kWPN_FIRE)
					SetShotRndSeed(ShotRndSeed);
				IR_OnKeyboardPress(cmd);
			}
			else
				IR_OnKeyboardRelease(cmd);
		}
		break;
	case GEG_PLAYER_ITEM2SLOT:
	case GEG_PLAYER_ITEM2BELT:
	case GEG_PLAYER_ITEM2RUCK:
	case GEG_PLAYER_ITEM_EAT:
	case GEG_PLAYER_ACTIVATEARTEFACT:
		{
			P.r_u16		(id);
			CObject* O	= Level().Objects.net_Find	(id);
			if(!O)		break;
			R_ASSERT	(!O->getDestroy());

			switch (type)
			{
			case GEG_PLAYER_ITEM2SLOT:	 
				inventory().Slot(smart_cast<CInventoryItem*>(O)); 
				break;
			case GEG_PLAYER_ITEM2BELT:	 
				inventory().Belt(smart_cast<CInventoryItem*>(O)); 
				break;
			case GEG_PLAYER_ITEM2RUCK:	 
				inventory().Ruck(smart_cast<CInventoryItem*>(O)); 
				break;
			case GEG_PLAYER_ITEM_EAT:	 
				inventory().Eat(smart_cast<CInventoryItem*>(O)); 
				break;
			case GEG_PLAYER_ACTIVATEARTEFACT:
				{
					CArtefact* pArtefact		= smart_cast<CArtefact*>(O);
					pArtefact->ActivateArtefact	();
				}break;
			}
		}break;
	case GEG_PLAYER_ACTIVATE_SLOT:
		{
			u32							slot_id;
			P.r_u32						(slot_id);

			inventory().Activate		(slot_id);
								  
		}break;

	case GEG_PLAYER_DISABLE_SPRINT:
		{
			s8 cmd				= P.r_s8();
			m_block_sprint_counter = m_block_sprint_counter+cmd;
			Msg("m_block_sprint_counter=%d",m_block_sprint_counter);
			if(m_block_sprint_counter>0)
			{
				mstate_wishful	&=~mcSprint;
			}
		}break;

	case GEG_PLAYER_WEAPON_HIDE_STATE:
		{
			u32 State		= P.r_u32();
			BOOL	Set		= !!P.r_u8();
			inventory().SetSlotsBlocked	((u16)State, !!Set);
		}break;
	case GE_MOVE_ACTOR:
		{
			Fvector NewPos, NewRot;
			P.r_vec3(NewPos);
			P.r_vec3(NewRot);
			
			MoveActor(NewPos, NewRot);
		}break;
	case GE_ACTOR_MAX_POWER:
		{
			conditions().MaxPower();
			conditions().ClearWounds();
			ClearBloodWounds();
		}break;
	case GE_ACTOR_MAX_HEALTH:
		{
			SetfHealth(GetMaxHealth());
		}break;
	case GEG_PLAYER_ATTACH_HOLDER:
		{
			u16 id = P.r_u16();
			CObject* O	= Level().Objects.net_Find	(id);
			if (!O){
				Msg("! Error: No object to attach holder [%d]", id);
				break;
			}
			VERIFY(m_holder==NULL);
			CHolderCustom*	holder = smart_cast<CHolderCustom*>(O);
			if(!holder->Engaged())	use_Holder		(holder);

		}break;
	case GEG_PLAYER_DETACH_HOLDER:
		{
			if			(!m_holder)	break;
			u16 id			= P.r_u16();
			CGameObject*	GO	= smart_cast<CGameObject*>(m_holder);
			VERIFY			(id==GO->ID());
			use_Holder		(NULL);
		}break;
	case GEG_PLAYER_PLAY_HEADSHOT_PARTICLE:
		{
			OnPlayHeadShotParticle(P);
		}break;
	case GE_ACTOR_JUMPING:
		{
			/*
			Fvector dir;
			P.r_dir(dir);
			float jump = P.r_float();
			NET_SavedAccel = dir;
			extern float NET_Jump;
			NET_Jump = jump;
			m_bInInterpolation = false;
			mstate_real |= mcJump;
			*/
		}break;
	}
}

void			CActor::MoveActor		(Fvector NewPos, Fvector NewDir)
{
	Fmatrix	M = XFORM();
	M.translate(NewPos);
	r_model_yaw				= NewDir.y;
	r_torso.yaw				= NewDir.y;
	r_torso.pitch			= -NewDir.x;
	unaffected_r_torso.yaw	= r_torso.yaw;
	unaffected_r_torso.pitch= r_torso.pitch;
	unaffected_r_torso.roll	= 0;//r_torso.roll;

	r_torso_tgt_roll		= 0;
	cam_Active()->Set		(-unaffected_r_torso.yaw,unaffected_r_torso.pitch,unaffected_r_torso.roll);
	ForceTransform(M);

	m_bInInterpolation = false;	
}
