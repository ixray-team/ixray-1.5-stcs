#pragma once
#include "../state.h"

template<typename _Object>
class CStateGroupAttackMoveToHomePoint : public CState<_Object> {
protected:
	typedef CState<_Object>		inherited;
	typedef CState<_Object>*	state_ptr;

	u32					m_target_node;
	bool				m_skip_camp;

public:
						CStateGroupAttackMoveToHomePoint(_Object *obj);
	virtual	void		initialize				();
	virtual void 		finalize				();
	virtual void 		critical_finalize		();
	virtual void		remove_links			(CObject* object_) { inherited::remove_links(object_);}

	virtual bool		check_start_conditions	();
	virtual bool		check_completion		();

	virtual	void		reselect_state			();
	virtual	void		setup_substates			();
};

#include "group_state_home_point_attack_inline.h"
