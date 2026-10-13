#include "StdAfx.h"
#include "state_arguments_functions.h"

namespace award_system
{
	//definitions of static members of functions_cf

	template<> ge_function<float> functions_cf<float>::ge_function = award_system::ge_function<float>();
	template<> le_function<float> functions_cf<float>::le_function = award_system::le_function<float>();

	template<> ge_function<u32> functions_cf<u32>::ge_function = award_system::ge_function<u32>();
	template<> le_function<u32> functions_cf<u32>::le_function = award_system::le_function<u32>();
} //namespace award_system
