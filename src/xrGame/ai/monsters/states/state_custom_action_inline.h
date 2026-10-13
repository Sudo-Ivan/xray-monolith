#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _Object\
>

#define CStateMonsterCustomActionAbstract CStateMonsterCustomAction<_Object>

TEMPLATE_SPECIALIZATION
CStateMonsterCustomActionAbstract::CStateMonsterCustomAction(_Object* obj) : inherited(obj, &data)
{
}

TEMPLATE_SPECIALIZATION
CStateMonsterCustomActionAbstract::~CStateMonsterCustomAction()
{
}


TEMPLATE_SPECIALIZATION
void CStateMonsterCustomActionAbstract::execute()
{
	this->object->anim().m_tAction = this->data.action;
	this->object->anim().SetSpecParams(this->data.spec_params);

	if (this->data.sound_type != u32(-1))
	{
		if (this->data.sound_delay != u32(-1))
			this->object->sound().play(this->data.sound_type, 0, 0, this->data.sound_delay);
		else
			this->object->sound().play(this->data.sound_type);
	}
}

TEMPLATE_SPECIALIZATION
bool CStateMonsterCustomActionAbstract::check_completion()
{
	if (this->data.time_out)
	{
		if (this->time_state_started + this->data.time_out < time()) return true;
	}

	return false;
}

#undef TEMPLATE_SPECIALIZATION
#undef CStateMonsterCustomActionAbstract
