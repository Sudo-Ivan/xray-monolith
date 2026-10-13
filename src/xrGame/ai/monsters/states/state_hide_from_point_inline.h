#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterHideFromPointAbstract CStateMonsterHideFromPoint<_Object>
#define DIST_TO_PATH_END		1.5f

TEMPLATE_SPECIALIZATION
void CStateMonsterHideFromPointAbstract::initialize()
{
	inherited::initialize();

	this->object->path().prepare_builder();
}

TEMPLATE_SPECIALIZATION
void CStateMonsterHideFromPointAbstract::execute()
{
	this->object->set_action(this->data.action.action);
	this->object->anim().SetSpecParams(this->data.action.spec_params);

	this->object->path().set_retreat_from_point(this->data.point);
	this->object->path().set_generic_parameters();

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
bool CStateMonsterHideFromPointAbstract::check_completion()
{
	if (this->data.action.time_out != 0)
	{
		if (this->time_state_started + this->data.action.time_out < Device.dwTimeGlobal)
			return true;
	}

	//if (!fis_zero(data.distance)) {
	//	if (object->Position().distance_to(data.point) > data.distance) 
	//		return true;
	//}

	return false;
}

#undef DIST_TO_PATH_END
#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterHideFromPointAbstract
