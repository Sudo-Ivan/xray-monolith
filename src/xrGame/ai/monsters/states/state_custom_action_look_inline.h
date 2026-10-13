#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterCustomActionLookAbstract CStateMonsterCustomActionLook<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterCustomActionLookAbstract::CStateMonsterCustomActionLook(_Object* obj) : inherited(obj, &data)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterCustomActionLookAbstract::~CStateMonsterCustomActionLook()
{
}


TEMPLATE_SPECIALIZATION
void CStateMonsterCustomActionLookAbstract::execute()
{
	this->object->set_action(this->data.action);
	this->object->anim().SetSpecParams(this->data.spec_params);
	this->object->dir().face_target(this->data.point);

	if (this->data.sound_type != u32(-1))
	{
		if (this->data.sound_delay != u32(-1))
			this->object->sound().play(this->data.sound_type, 0, 0, this->data.sound_delay);
		else
			this->object->sound().play(this->data.sound_type);
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterCustomActionLookAbstract::check_completion()
{
	if (this->data.time_out)
	{
		if (this->time_state_started + this->data.time_out < time()) return true;
	}

	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterCustomActionLookAbstract
