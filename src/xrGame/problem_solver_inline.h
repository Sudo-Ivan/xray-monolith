////////////////////////////////////////////////////////////////////////////
//	Module 		: problem_solver_inline.h
//	Created 	: 24.02.2004
//  Modified 	: 24.02.2004
//	Author		: Dmitriy Iassenev
//	Description : Problem solver inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template<\
	typename _operator_condition,\
	typename _operator,\
	typename _condition_state,\
	typename _condition_evaluator,\
	typename _operator_id_type,\
	bool	 _reverse_search,\
	typename _operator_ptr_t,\
	typename _condition_evaluator_ptr_t\
>

#define CProblemSolverAbstract \
	CProblemSolver<\
		_operator_condition,\
		_operator,\
		_condition_state,\
		_condition_evaluator,\
		_operator_id_type,\
		_reverse_search,\
		_operator_ptr_t,\
		_condition_evaluator_ptr_t\
	>

TEMPLATE_SPECIALIZATION
IC CProblemSolverAbstract::CProblemSolver()
{
	this->init();
}

TEMPLATE_SPECIALIZATION
CProblemSolverAbstract::~CProblemSolver()
{
	this->clear();
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::clear()
{
	while (!this->m_operators.empty())
		this->remove_operator(this->m_operators.back().m_operator_id);

	while (!this->m_evaluators.empty())
		this->remove_evaluator((*(this->m_evaluators.end() - 1)).first);
}

TEMPLATE_SPECIALIZATION
void CProblemSolverAbstract::init()
{
}

TEMPLATE_SPECIALIZATION
void CProblemSolverAbstract::setup()
{
	this->m_target_state.clear();
	this->m_current_state.clear();
	this->m_temp.clear();
	m_solution.clear();
	this->m_applied = false;
	this->m_solution_changed = false;
	this->m_actuality = true;
	this->m_failed = false;
}

TEMPLATE_SPECIALIZATION
IC bool CProblemSolverAbstract::actual() const
{
	if (!this->m_actuality)
		return (false);

	typename xr_vector<COperatorCondition>::const_iterator I = this->current_state().conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator E = this->current_state().conditions().end();
	typename CProblemSolverAbstract::EVALUATORS::const_iterator i = this->evaluators().begin();
	typename CProblemSolverAbstract::EVALUATORS::const_iterator e = this->evaluators().end();
	for (; I != E; ++I)
	{
		if ((*i).first < (*I).condition())
			i = std::lower_bound(i, e, (*I).condition(), this->evaluators().value_comp());
		VERIFY(i != e);
		VERIFY((*i).first == (*I).condition());
		if ((*i).second->evaluate() != (*I).value())
			return (false);
	}
	return (true);
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::add_operator(const _edge_type& operator_id, _operator_ptr _operator_p)
{
	typename CProblemSolverAbstract::OPERATOR_VECTOR::iterator I = std::lower_bound(this->m_operators.begin(), this->m_operators.end(), operator_id);
	THROW((I == this->m_operators.end()) || ((*I).m_operator_id != operator_id));
#ifdef DEBUG
	validate_properties			(_operator_p->conditions());
	validate_properties			(_operator_p->effects());
#endif
	this->m_actuality = false;
	this->m_operators.insert(I, SOperator(operator_id, _operator_p));
}

#ifdef DEBUG
TEMPLATE_SPECIALIZATION
IC	void CProblemSolverAbstract::validate_properties	(const CState &conditions) const
{
	typename xr_vector<COperatorCondition>::const_iterator	I = conditions.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator	E = conditions.conditions().end();
	for ( ; I != E; ++I) {
		if (this->evaluators().find((*I).condition()) == this->evaluators().end()) {
			Msg		("! cannot find corresponding evaluator to the property with id %d",(*I).condition());
			THROW	(this->evaluators().find((*I).condition()) != this->evaluators().end());
		}
	}
}
#endif

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::remove_operator(const _edge_type& operator_id)
{
	typename CProblemSolverAbstract::OPERATOR_VECTOR::iterator I = std::lower_bound(this->m_operators.begin(), this->m_operators.end(), operator_id);
	THROW(this->m_operators.end() != I);
	try
	{
		delete_data((*I).m_operator);
	}
	catch (...)
	{
		(*I).m_operator = 0;
	}
	this->m_actuality = false;
	this->m_operators.erase(I);
}

TEMPLATE_SPECIALIZATION
IC const typename CProblemSolverAbstract::OPERATOR_VECTOR&CProblemSolverAbstract::operators() const
{
	return (this->m_operators);
}

// states
TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::set_target_state(const CState& state)
{
	this->m_actuality = this->m_actuality && (this->m_target_state == state);
	this->m_target_state = state;
}

TEMPLATE_SPECIALIZATION
IC const typename CProblemSolverAbstract::CState&CProblemSolverAbstract::current_state() const
{
	return (this->m_current_state);
}

TEMPLATE_SPECIALIZATION
IC const typename CProblemSolverAbstract::CState&CProblemSolverAbstract::target_state() const
{
	return (this->m_target_state);
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::add_evaluator(const _condition_type& condition_id, _condition_evaluator_ptr evaluator)
{
	THROW(this->evaluators().end() == this->evaluators().find(condition_id));
	this->m_evaluators.insert(std::make_pair(condition_id, evaluator));
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::remove_evaluator(const _condition_type& condition_id)
{
	typename CProblemSolverAbstract::EVALUATORS::iterator I = this->m_evaluators.find(condition_id);
	THROW(I != this->m_evaluators.end());
	try
	{
		delete_data((*I).second);
	}
	catch (...)
	{
		(*I).second = 0;
	}
	this->m_evaluators.erase(I);
	this->m_actuality = false;
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_condition_evaluator_ptr CProblemSolverAbstract::evaluator(
	const _condition_type& condition_id) const
{
	typename CProblemSolverAbstract::EVALUATORS::const_iterator I = this->evaluators().find(condition_id);
	THROW(this->evaluators().end() != I);
	return ((*I).second);
}

TEMPLATE_SPECIALIZATION
IC const typename CProblemSolverAbstract::EVALUATORS&CProblemSolverAbstract::evaluators() const
{
	return (this->m_evaluators);
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::evaluate_condition(typename xr_vector<COperatorCondition>::const_iterator& I,
                                                   typename xr_vector<COperatorCondition>::const_iterator& E,
                                                   const _condition_type& condition_id) const
{
	size_t index = I - this->m_current_state.conditions().begin();
	this->m_current_state.add_condition(I, COperatorCondition(condition_id, this->evaluator(condition_id)->evaluate()));
	I = this->m_current_state.conditions().begin() + index;
	E = this->m_current_state.conditions().end();
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_edge_value_type CProblemSolverAbstract::get_edge_weight(
	const _index_type& vertex_index0, const _index_type& vertex_index1, const const_iterator& i) const
{
	_edge_value_type current, min;
	current = (*i).m_operator->weight(vertex_index1, vertex_index0);
	min = (*i).m_operator->min_weight();
	THROW(current >= min);
	return (current);
}

TEMPLATE_SPECIALIZATION
IC bool CProblemSolverAbstract::is_accessible(const _index_type& vertex_index) const
{
	return (this->m_applied);
}

TEMPLATE_SPECIALIZATION
IC const typename CProblemSolverAbstract::_index_type&CProblemSolverAbstract::value(
	const _index_type& vertex_index, const_iterator& i, bool reverse_search) const
{
	if (reverse_search)
	{
		if ((*i).m_operator->applicable_reverse((*i).m_operator->effects(), (*i).m_operator->conditions(), vertex_index)
		)
			this->m_applied = (*i).m_operator->apply_reverse(vertex_index, (*i).m_operator->effects(), this->m_temp,
			                                           (*i).m_operator->conditions());
		else
			this->m_applied = false;
	}
	else
	{
		if ((*i).m_operator->applicable(vertex_index, this->current_state(), (*i).m_operator->conditions(), *this))
		{
			(*i).m_operator->apply(vertex_index, (*i).m_operator->effects(), this->m_temp, this->m_current_state, *this);
			this->m_applied = true;
		}
		else
			this->m_applied = false;
	}
	return (this->m_temp);
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::begin(const _index_type& vertex_index, const_iterator& b, const_iterator& e) const
{
	b = this->m_operators.begin();
	e = this->m_operators.end();
}

TEMPLATE_SPECIALIZATION
IC bool CProblemSolverAbstract::is_goal_reached(const _index_type& vertex_index) const
{
	return (is_goal_reached_impl<reverse_search>(vertex_index));
}

TEMPLATE_SPECIALIZATION
IC bool CProblemSolverAbstract::is_goal_reached_impl(const _index_type& vertex_index) const
{
	STATIC_CHECK(!reverse_search, This_function_cannot_be_used_in_the_REVERSE_search);
	typename xr_vector<COperatorCondition>::const_iterator I = vertex_index.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator E = vertex_index.conditions().end();
	typename xr_vector<COperatorCondition>::const_iterator i = this->target_state().conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator e = this->target_state().conditions().end();
	typename xr_vector<COperatorCondition>::const_iterator II = this->current_state().conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator EE = this->current_state().conditions().end();
	for (; (i != e) && (I != E);)
	{
		if ((*I).condition() < (*i).condition())
		{
			++I;
		}
		else if ((*I).condition() > (*i).condition())
		{
			for (; (II != EE) && ((*II).condition() < (*i).condition());)
				++II;
			if ((II == EE) || ((*II).condition() > (*i).condition()))
				evaluate_condition(II, EE, (*i).condition());
			if ((*II).value() != (*i).value())
				return (false);
			++II;
			++i;
		}
		else
		{
			if ((*I).value() != (*i).value())
				return (false);
			++I;
			++i;
		}
	}

	if (I == E)
	{
		I = II;
		E = EE;
	}
	else
		return (true);

	for (; i != e;)
	{
		if ((I == E) || ((*I).condition() > (*i).condition()))
			evaluate_condition(I, E, (*i).condition());

		if ((*I).condition() < (*i).condition())
			++I;
		else
		{
			VERIFY((*I).condition() == (*i).condition());
			if ((*I).value() != (*i).value())
				return (false);
			++I;
			++i;
		}
	}
	return (true);
}

TEMPLATE_SPECIALIZATION
IC bool CProblemSolverAbstract::is_goal_reached_impl(const _index_type& vertex_index, bool) const
{
	STATIC_CHECK(reverse_search, This_function_cannot_be_used_in_the_STRAIGHT_search);
	typename xr_vector<COperatorCondition>::const_iterator I = this->m_current_state.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator E = this->m_current_state.conditions().end();
	typename xr_vector<COperatorCondition>::const_iterator i = vertex_index.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator e = vertex_index.conditions().end();
	for (; i != e;)
	{
		if ((I == E) || ((*I).condition() > (*i).condition()))
			evaluate_condition(I, E, (*i).condition());

		if ((*I).condition() < (*i).condition())
			++I;
		else
		{
			if ((*I).value() != (*i).value())
				return (false);
			++I;
			++i;
		}
	}
	return (true);
}

TEMPLATE_SPECIALIZATION
IC const xr_vector<typename CProblemSolverAbstract::_edge_type>&CProblemSolverAbstract::solution() const
{
	return (m_solution);
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_operator_ptr CProblemSolverAbstract::get_operator(const _edge_type& operator_id)
{
	typename CProblemSolverAbstract::OPERATOR_VECTOR::iterator I = std::lower_bound(this->m_operators.begin(), this->m_operators.end(), operator_id);
	if (this->m_operators.end() == I) return 0;
	THROW(this->m_operators.end() != I);
	return ((*I).get_operator());
}

TEMPLATE_SPECIALIZATION
IC void CProblemSolverAbstract::solve()
{
#ifndef AI_COMPILER
	this->m_solution_changed = false;

	if (actual())
		return;

	this->m_actuality = true;
	this->m_solution_changed = true;
	this->m_current_state.clear();

	this->m_failed = !
		ai().graph_engine().search(
			*this,
			reverse_search ? this->target_state() : this->current_state(),
			reverse_search ? this->current_state() : this->target_state(),
			&m_solution,
			GraphEngineSpace::CSolverBaseParameters(
				GraphEngineSpace::_solver_dist_type(-1),
				GraphEngineSpace::_solver_condition_type(-1),
				8000
			)
		);
#endif
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_edge_value_type CProblemSolverAbstract::estimate_edge_weight(
	const _index_type& condition) const
{
	return (helper<reverse_search>::estimate_edge_weight_impl(*this, condition));
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_edge_value_type CProblemSolverAbstract::estimate_edge_weight_impl(
	const _index_type& condition) const
{
	STATIC_CHECK(!reverse_search, This_function_cannot_be_used_in_the_REVERSE_search);
	_edge_value_type result = 0;
	typename xr_vector<COperatorCondition>::const_iterator I = this->target_state().conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator E = this->target_state().conditions().end();
	typename xr_vector<COperatorCondition>::const_iterator i = condition.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator e = condition.conditions().end();
	for (; (I != E) && (i != e);)
		if ((*I).condition() < (*i).condition())
		{
			++result;
			++I;
		}
		else if ((*I).condition() > (*i).condition())
			++i;
		else
		{
			if ((*I).value() != (*i).value())
				++result;
			++I;
			++i;
		}
	return (result + _edge_value_type(E - I));
}

TEMPLATE_SPECIALIZATION
IC typename CProblemSolverAbstract::_edge_value_type CProblemSolverAbstract::estimate_edge_weight_impl(
	const _index_type& condition, bool) const
{
	STATIC_CHECK(reverse_search, This_function_cannot_be_used_in_the_STRAIGHT_search);
	_edge_value_type result = 0;
	typename xr_vector<COperatorCondition>::const_iterator I = this->current_state().conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator E = this->current_state().conditions().end();
	typename xr_vector<COperatorCondition>::const_iterator i = condition.conditions().begin();
	typename xr_vector<COperatorCondition>::const_iterator e = condition.conditions().end();
	for (; (i != e);)
	{
		if ((I == E) || ((*I).condition() > (*i).condition()))
			evaluate_condition(I, E, (*i).condition());

		if ((*I).condition() < (*i).condition())
			++I;
		else
		{
			VERIFY((*I).condition() == (*i).condition());
			if ((*I).value() != (*i).value())
				++result;
			++I;
			++i;
		}
	}
	return (result);
}

#undef TEMPLATE_SPECIALIZATION
#undef CProblemSolverAbstract
