#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterMoveToPointAbstract CStateMonsterMoveToPoint<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToPointAbstract::initialize()
{
	inherited::initialize();
	this->object->path().prepare_builder();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToPointAbstract::execute()
{
	this->object->set_action(this->data.action.action);
	this->object->anim().SetSpecParams(this->data.action.spec_params);

	this->object->path().set_target_point(this->data.point, this->data.vertex);
	this->object->path().set_generic_parameters();
	this->object->path().set_distance_to_end(this->data.completion_dist);

	if (this->data.accelerated)
	{
		this->object->anim().accel_activate(EAccelType(this->data.accel_type));
		this->object->anim().accel_set_braking(this->data.braking);
	}

	if (this->data.action.sound_type != u32(-1))
	{
		this->object->set_state_sound(this->data.action.sound_type, this->data.action.sound_delay == u32(-1));
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterMoveToPointAbstract::check_completion()
{
	if (this->data.action.time_out != 0)
	{
		if (this->time_state_started + this->data.action.time_out < Device.dwTimeGlobal) return true;
	}

	bool real_path_end = ((fis_zero(this->data.completion_dist))
		                      ? (this->data.point.distance_to_xz(this->object->Position()) < ai().level_graph().header().cell_size()
		                      )
		                      : true);
	if (this->object->control().path_builder().is_path_end(this->data.completion_dist) && real_path_end) return true;

	return false;
}


//////////////////////////////////////////////////////////////////////////
// CStateMonsterMoveToPointEx with path rebuild options
//////////////////////////////////////////////////////////////////////////

#define CStateMonsterMoveToPointExAbstract CStateMonsterMoveToPointEx<_Object>

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToPointExAbstract::initialize()
{
	inherited::initialize();
	this->object->path().prepare_builder();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterMoveToPointExAbstract::execute()
{
	this->object->set_action(this->data.action.action);
	this->object->anim().SetSpecParams(this->data.action.spec_params);

	this->object->path().set_target_point(this->data.point, this->data.vertex);
	this->object->path().set_rebuild_time(this->data.time_to_rebuild);
	this->object->path().set_distance_to_end(this->data.completion_dist);
	this->object->path().set_use_covers();
	this->object->path().set_cover_params(5.f, 30.f, 1.f, 30.f);

	if (this->data.target_direction.magnitude() > 0.0001f)
	{
		this->object->path().set_use_dest_orient(true);
		this->object->path().set_dest_direction(this->data.target_direction);
	}
	else
	{
		this->object->path().set_use_dest_orient(false);
	}

	if (this->data.accelerated)
	{
		this->object->anim().accel_activate(EAccelType(this->data.accel_type));
		this->object->anim().accel_set_braking(this->data.braking);
	}

	if (this->data.action.sound_type != u32(-1))
	{
		this->object->set_state_sound(this->data.action.sound_type, this->data.action.sound_delay == u32(-1));
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterMoveToPointExAbstract::check_completion()
{
	if (this->data.action.time_out != 0)
	{
		if (this->time_state_started + this->data.action.time_out < Device.dwTimeGlobal)
			return true;
	}

	Fvector const self_pos = this->object->Position();
	float const dist_to_target = this->data.point.distance_to_xz(self_pos);
	float const completion_dist = _max(this->data.completion_dist, ai().level_graph().header().cell_size());

	if (Device.dwTimeGlobal < this->time_state_started + 200)
	{
		if (dist_to_target > completion_dist)
			return false;
	}

	bool const real_path_end = fis_zero(this->data.completion_dist)
		                           ? dist_to_target < ai().level_graph().header().cell_size()
		                           : true;

	if (this->object->control().path_builder().is_path_end(this->data.completion_dist) && real_path_end)
		return true;

	return false;
}
