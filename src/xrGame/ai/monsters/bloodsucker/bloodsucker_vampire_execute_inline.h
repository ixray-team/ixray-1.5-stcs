#pragma once

#include "../../../../Include/xrRender/KinematicsAnimated.h"
#include "../../../actor.h"
#include "../../../../xrEngine/CameraBase.h"

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateBloodsuckerVampireExecuteAbstract CStateBloodsuckerVampireExecute<_Object>

#define VAMPIRE_TIME_HOLD		4000
#define VAMPIRE_HIT_IMPULSE		400.f
#define VAMPIRE_MIN_DIST		0.5f
#define VAMPIRE_MAX_DIST		1.5f

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::initialize()
{
	inherited::initialize					();

	this->object->CControlledActor::install		();

	look_head				();

	m_action				= eActionPrepare;
	time_vampire_started	= 0;

	this->object->stop_invisible_predator	();

	m_effector_activated			= false;
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute()
{
	if (!this->object->CControlledActor::is_turning() && !m_effector_activated) {
		this->object->ActivateVampireEffector	();
		m_effector_activated			= true;
	}
	
	look_head							();

	switch (m_action) {
		case eActionPrepare:
			execute_vampire_prepare();
			m_action = eActionContinue;
			break;

		case eActionContinue:
			execute_vampire_continue();
			break;

		case eActionFire:
			execute_vampire_hit();
			m_action = eActionWaitTripleEnd;
			break;

		case eActionWaitTripleEnd:
			if (!this->object->com_man().ta_is_active()) {
				m_action = eActionCompleted; 
			}

		case eActionCompleted:
			break;
	}

	this->object->set_action			(ACT_STAND_IDLE);
	this->object->dir().face_target	(this->object->EnemyMan.get_enemy());
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::finalize()
{
	inherited::finalize();

	this->object->start_invisible_predator	();

	if (this->object->CControlledActor::is_controlling())
		this->object->CControlledActor::release		();
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::critical_finalize()
{
	inherited::critical_finalize();

	if (this->object->CControlledActor::is_controlling())
		this->object->CControlledActor::release		();
	
	this->object->start_invisible_predator	();
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireExecuteAbstract::check_start_conditions()
{
	const CEntityAlive	*enemy = this->object->EnemyMan.get_enemy();
	
	// проверить дистанцию
	float dist		= this->object->MeleeChecker.distance_to_enemy	(enemy);
	if ((dist > VAMPIRE_MAX_DIST) || (dist < VAMPIRE_MIN_DIST))	return false;

	// проверить направление на врага
	if (!this->object->control().direction().is_face_target(enemy, PI_DIV_6)) return false;

	return true;
}

TEMPLATE_SPECIALIZATION
bool CStateBloodsuckerVampireExecuteAbstract::check_completion()
{
	return (m_action == eActionCompleted);
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_prepare()
{
	this->object->com_man().ta_activate		(this->object->anim_triple_vampire);
	time_vampire_started				= Device.dwTimeGlobal;
	
	this->object->sound().play(CAI_Bloodsucker::eVampireGrasp);
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_continue()
{
	if (this->object->Position().distance_to(Actor()->Position()) > 2.f) {
		this->object->com_man().ta_deactivate();
		m_action = eActionCompleted;
		return;
	}
	
	this->object->sound().play(CAI_Bloodsucker::eVampireSucking);

	// проверить на грави удар
	if (time_vampire_started + VAMPIRE_TIME_HOLD < Device.dwTimeGlobal) {
		m_action = eActionFire;
	}
}

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::execute_vampire_hit()
{
	this->object->com_man().ta_pointbreak				();
	this->object->sound().play						(CAI_Bloodsucker::eVampireHit);
	this->object->SatisfyVampire						();
}

//////////////////////////////////////////////////////////////////////////

TEMPLATE_SPECIALIZATION
void CStateBloodsuckerVampireExecuteAbstract::look_head()
{
	IKinematics *pK = smart_cast<IKinematics*>(this->object->Visual());
	Fmatrix bone_transform;
	bone_transform = pK->LL_GetTransform(pK->LL_BoneID("bip01_head"));	

	Fmatrix global_transform;
	global_transform.mul_43(this->object->XFORM(),bone_transform);

	this->object->CControlledActor::look_point	(global_transform.c);
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateBloodsuckerVampireExecuteAbstract

