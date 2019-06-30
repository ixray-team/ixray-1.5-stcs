////////////////////////////////////////////////////////////////////////////
//	Module 		: aimers_base.cpp
//	Created 	: 04.04.2008
//  Modified 	: 08.04.2008
//	Author		: Dmitriy Iassenev
//	Description : aimers base class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aimers_base.h"
#include "gameobject.h"
#include "../include/xrrender/kinematics.h"
#include "animation_movement_controller.h"

using aimers::base;

base::base						(
		CGameObject* object,
		LPCSTR animation_id,
		bool animation_start,
		Fvector const& target
	) :
	m_object				( *object ),
	m_kinematics			( smart_cast<IKinematics&>( *object->Visual() ) ),
	m_animated				( smart_cast<IKinematicsAnimated&>( *object->Visual() ) ),
	m_target				( target ),
	m_animation_id			( m_animated.LL_MotionID( animation_id ) ),
	m_animation_start		( animation_start )
{
	animation_movement_controller const*	controller = m_object.animation_movement();
	if (!controller)
		m_start_transform	= m_object.XFORM();
	else
		m_start_transform	= controller->start_transform();

	R_ASSERT				( _valid(m_start_transform) );
}

void base::callback				(CBoneInstance* bone)
{
	VERIFY					(bone);

	Fmatrix*				rotation = static_cast<Fmatrix*>(bone->callback_param());
	VERIFY					(rotation);
	R_ASSERT2				( _valid( *rotation ), "base::callback[rotation] " );

	Fvector	position		= bone->mTransform.c;
	bone->mTransform.mulA_43(*rotation);
	bone->mTransform.c		= position;
	R_ASSERT2				( _valid( bone->mTransform ), "base::callback " );
}

void base::aim_at_position		(
		Fvector const& bone_position,
		Fvector const& object_position,
		Fvector object_direction,
		Fmatrix& result
	)
{
#if 0
	Msg									(
		"[%d][%s] bone_position[%f][%f][%f] object_position[%f][%f][%f] object_direction[%f][%f][%f]",
		Device.dwFrame,
		m_animated.LL_MotionDefName_dbg(m_animation_id).first,
		VPUSH(bone_position),
		VPUSH(object_position),
		VPUSH(object_direction)
	);
#endif // #if 0

	R_ASSERT2							(
		_valid(bone_position),
		make_string(
			"[%f][%f][%f]",
			VPUSH(bone_position)
		)
	);
	R_ASSERT2							(
		_valid(object_position),
		make_string(
			"[%f][%f][%f]",
			VPUSH(object_position)
		)
	);
	R_ASSERT2							(
		_valid(object_direction),
		make_string(
			"[%f][%f][%f]",
			VPUSH(object_direction)
		)
	);
	R_ASSERT2							(
		_valid(m_target),
		make_string(
			"[%f][%f][%f]",
			VPUSH(m_target)
		)
	);

	VERIFY2								(
		object_direction.square_magnitude() > EPS_L,
		make_string("[%f]", object_direction.square_magnitude())
	);

	object_direction.normalize			();

	Fvector	const object2bone			= Fvector().sub(bone_position, object_position);
	float const offset					= object2bone.dotproduct(object_direction);
	Fvector const current_point			= Fvector().mad(object_position, object_direction, offset);

	Fvector bone2current				= Fvector().sub(current_point, bone_position);
	if (bone2current.magnitude() < EPS_L)
		bone2current.set				( 0.f, 0.f, EPS_L );

	float const sphere_radius_sqr		= bone2current.square_magnitude();

	Fvector direction_target			= Fvector().sub(m_target, bone_position);
	if (direction_target.magnitude() < EPS_L)
		direction_target.set			( 0.f, 0.f, EPS_L );

	float const invert_magnitude		= 1.f/direction_target.magnitude();
	direction_target.mul				(invert_magnitude);
	VERIFY2								(
		fsimilar(direction_target.magnitude(), 1.f),
		make_string(
			"%f",
			direction_target.magnitude()
		)
	);

	float const to_circle_center		= sphere_radius_sqr*invert_magnitude;
	Fvector const circle_center			= Fvector().mad(bone_position, direction_target, to_circle_center);

	Fplane const plane					= Fplane().build(circle_center, direction_target);
	Fvector								projection;
	plane.project						(projection, current_point);

	Fvector projection2circle_center	= Fvector().sub(projection, circle_center);
	if (projection2circle_center.magnitude() < EPS_L)
		projection2circle_center.set	( 0.f, 0.f, EPS_L );
	Fvector const center2projection_direction	= projection2circle_center.normalize();

	float circle_radius_sqr				= sphere_radius_sqr - _sqr(to_circle_center);
	if (circle_radius_sqr < 0.f)
		circle_radius_sqr				= 0.f;

	float const circle_radius			= _sqrt(circle_radius_sqr);
	Fvector const target_point			= Fvector().mad(circle_center, center2projection_direction, circle_radius);
	Fvector const current_direction		= Fvector(bone2current).normalize();

	Fvector target2bone					= Fvector().sub(target_point,  bone_position);
	if (target2bone.magnitude() < EPS_L)
		target2bone.set					( 0.f, 0.f, EPS_L);
	Fvector const target_direction		= target2bone.normalize();

	Fmatrix								transform0;
	{
		Fvector							cross_product = Fvector().crossproduct(current_direction, target_direction);
		float const sin_alpha			= cross_product.magnitude();
		if (!fis_zero(sin_alpha)) {
			float const cos_alpha		= current_direction.dotproduct(target_direction);
			transform0.rotation			(cross_product.div(sin_alpha), atan2f(sin_alpha, cos_alpha));
		}
		else {
			float const dot_product		= current_direction.dotproduct(target_direction);
			if (fsimilar(_abs(dot_product), 0.f))
				transform0.identity		();
			else {
				VERIFY					(fsimilar(_abs(dot_product), 1.f));
				cross_product.crossproduct	(current_direction, direction_target);
				float const sin_alpha2	= cross_product.magnitude();
				if (!fis_zero(sin_alpha2))
					transform0.rotation	(cross_product.div(sin_alpha2), dot_product > 0.f ? 0.f : PI);
				else
					transform0.rotation	( Fvector().set(0.f, 0.f, 1.f) , dot_product > 0.f ? 0.f : PI);
			}
		}
	}

	Fmatrix								transform1;
	{
		Fvector target2target_point		= Fvector().sub(m_target, target_point);
		if (target2target_point.magnitude() < EPS_L)
			target2target_point.set		(0.f, 0.f, EPS_L);
		Fvector const new_direction		= target2target_point.normalize();

		Fvector							old_direction;
		transform0.transform_dir		(old_direction, object_direction);
		Fvector 						cross_product = Fvector().crossproduct(old_direction, new_direction);
		float const sin_alpha			= cross_product.magnitude();
		if (!fis_zero(sin_alpha)) {
			float const cos_alpha		= old_direction.dotproduct(new_direction);
			transform1.rotation			(cross_product.div(sin_alpha), atan2f(sin_alpha, cos_alpha));
		}
		else {
			float const dot_product		= current_direction.dotproduct(target_direction);
			if (fsimilar(_abs(dot_product), 0.f))
				transform1.identity		();
			else {
				VERIFY					(fsimilar(_abs(dot_product), 1.f));
				transform1.rotation		(target_direction, dot_product > 0.f ? 0.f : PI);
			}
		}
	}

	R_ASSERT							( _valid(transform0) );
	R_ASSERT							( _valid(transform1) );
	result.mul_43						(transform1, transform0);
	R_ASSERT							( _valid(result) );
}
