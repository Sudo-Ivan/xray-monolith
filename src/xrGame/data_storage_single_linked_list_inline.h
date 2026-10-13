////////////////////////////////////////////////////////////////////////////
//	Module 		: data_storage_signle_linked_list_inline.h
//	Created 	: 21.03.2002
//  Modified 	: 26.02.2004
//	Author		: Dmitriy Iassenev
//	Description : Single linked list data storage inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION \
	template <bool sorted>\
	template <\
		typename _data_storage,\
		template <typename _T> class _vertex\
	>

#define CSingleLinkedList	CDataStorageSingleLinkedList<sorted>::CDataStorage<_data_storage,_vertex>

TEMPLATE_SPECIALIZATION
IC CSingleLinkedList::CDataStorage(const u32 vertex_count, const _dist_type _max_distance) :
	inherited(vertex_count)
{
	this->m_max_distance = _max_distance;
}

TEMPLATE_SPECIALIZATION
CSingleLinkedList::~CDataStorage()
{
}

TEMPLATE_SPECIALIZATION
IC void CSingleLinkedList::init()
{
	inherited::init();
	ZeroMemory(m_list_data, 2*sizeof(CGraphVertex));
	this->m_list_head = m_list_data;
	this->m_list_tail = m_list_data + 1;
	this->m_list_head->next() = this->m_list_tail;
	this->m_list_tail->f() = this->m_max_distance;
}

TEMPLATE_SPECIALIZATION
IC bool CSingleLinkedList::is_opened_empty() const
{
	return (this->m_list_head->next() == this->m_list_tail);
}

TEMPLATE_SPECIALIZATION
IC void CSingleLinkedList::add_opened(CGraphVertex& vertex)
{
	inherited::add_opened(vertex);
	if (!this->sorted)
	{
		vertex.next() = this->m_list_head->next();
		this->m_list_head->next() = &vertex;
	}
	else
	{
		for (CGraphVertex* i = this->m_list_head; ; i = i->next())
			if (i->next()->f() >= vertex.f())
			{
				vertex.next() = i->next();
				i->next() = &vertex;
				break;
			}
	}
}

TEMPLATE_SPECIALIZATION
IC void CSingleLinkedList::decrease_opened(CGraphVertex& vertex, const _dist_type value)
{
	VERIFY(!is_opened_empty());

	if (!this->sorted)
		return;

	for (CGraphVertex* i = this->m_list_head; ; i = i->next())
		if (&vertex == i->next())
		{
			if (i->f() <= vertex.f())
				return;
			i->next() = i->next()->next();
			break;
		}

	if (vertex.f() <= this->m_list_head->next()->f())
	{
		vertex.next() = this->m_list_head->next();
		this->m_list_head->next() = &vertex;
	}
	else
		for (i = this->m_list_head; ; i = i->next())
			if (i->next()->f() >= vertex.f())
			{
				vertex.next() = i->next();
				i->next() = &vertex;
				break;
			}
}

TEMPLATE_SPECIALIZATION
IC void CSingleLinkedList::remove_best_opened()
{
	VERIFY(!is_opened_empty());
	this->m_list_head->next() = this->m_list_head->next()->next();
}

TEMPLATE_SPECIALIZATION
IC void CSingleLinkedList::add_best_closed()
{
	VERIFY(!is_opened_empty());
	inherited::add_closed(*this->m_list_head->next());
}

TEMPLATE_SPECIALIZATION
IC typename CSingleLinkedList::CGraphVertex&CSingleLinkedList::get_best() const
{
	VERIFY(!is_opened_empty());

	if (this->sorted)
		return (*this->m_list_head->next());

	_dist_type fmin = this->m_max_distance;
	for (CGraphVertex *i = this->m_list_head, *best_prev = 0; i->next() != this->m_list_tail; i = i->next())
		if (i->next()->f() < fmin)
		{
			fmin = i->next()->f();
			best_prev = i;
		}

	VERIFY(best_prev);
	if (best_prev != this->m_list_head)
	{
		CGraphVertex* best = best_prev->next();
		best_prev->next() = best->next();
		best->next() = this->m_list_head->next();
		this->m_list_head->next() = best;
	}
	return (*this->m_list_head->next());
}

#undef TEMPLATE_SPECIALIZATION
#undef CSingleLinkedList
