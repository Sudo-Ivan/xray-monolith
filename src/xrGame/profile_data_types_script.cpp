#include "StdAfx.h"
#include "profile_data_types.h"

using namespace luabind;

#pragma optimize("s",on)

void profile_data_script_registrator::script_register(lua_State* L)
{
	using namespace gamespy_profile;
	module(L)
	[
		class_<award_data>("award_data")
		.def_readonly("m_count", &award_data::m_count)
		.def_readonly("m_last_reward_date", &award_data::m_last_reward_date),

		class_<all_awards_t::value_type>("award_pair_t")
		.def_readonly("first", &all_awards_t::value_type::first)
		.def_readonly("second", &all_awards_t::value_type::second),

		class_<all_best_scores_t::value_type>("best_scores_pair_t")
		.def_readonly("first", &all_best_scores_t::value_type::first)
		.def_readonly("second", &all_best_scores_t::value_type::second)
	];
}

#undef DEFINE_MIXED_DELEGATE_SCRIPT
#define DEFINE_MIXED_DELEGATE_SCRIPT(type, name_str) \
	template <> \
	void type::script_register(lua_State *L)\
	{\
		module(L)\
		[\
			class_<type>(name_str)\
				.def(						constructor<>())\
				.def(						constructor<type::lua_object_type, type::lua_function_type>())\
				.def("bind",				static_cast<void (type::*)(type::lua_object_type, type::lua_function_type)>(&type::bind))\
				.def("clear",				&type::clear)\
		];\
	};

DEFINE_MIXED_DELEGATE_SCRIPT(gamespy_profile::store_operation_cb, "store_operation_cb");
