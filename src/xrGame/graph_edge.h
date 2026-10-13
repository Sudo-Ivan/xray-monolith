////////////////////////////////////////////////////////////////////////////
//	Module 		: CEdge.h
//	Created 	: 14.01.2004
//  Modified 	: 19.02.2005
//	Author		: Dmitriy Iassenev
//	Description : Graph edge class template
////////////////////////////////////////////////////////////////////////////

#pragma once

template <typename _data_type, typename _vertex_id_type, typename _graph_type>
class CVertex;

// breaks the CVertex <-> CEdge instantiation cycle
template <typename _T>
struct _vertex_id_of
{
	typedef typename _T::_vertex_id_type type;
};

template <typename _D, typename _I, typename _G>
struct _vertex_id_of<CVertex<_D, _I, _G>>
{
	typedef _I type;
};

template <typename _edge_weight_type_t, typename _vertex_type_t>
class CEdgeBase
{
public:
	typedef _edge_weight_type_t _edge_weight_type;
	typedef _vertex_type_t _vertex_type;
	typedef typename _vertex_id_of<_vertex_type>::type _vertex_id_type;

private:
	_edge_weight_type m_weight;
	_vertex_type* m_vertex;

public:
	IC CEdgeBase(const _edge_weight_type& weight, _vertex_type* vertex);
	IC const _edge_weight_type& weight() const;
	IC _vertex_type* vertex() const;
	IC const _vertex_id_type& vertex_id() const;
};

template <typename _edge_weight_type, typename _vertex_type, typename _edge_data_type>
class CEdge : 
	public CEdgeBase<_edge_weight_type,_vertex_type> 
{
private:
	typedef CEdgeBase<_edge_weight_type, _vertex_type> inherited;
	using _vertex_id_type = typename inherited::_vertex_id_type;

private:
	_edge_data_type m_data;

public:
	IC CEdge(const _edge_weight_type& weight, _vertex_type* vertex);
	IC bool operator==(const _vertex_id_type& vertex_id) const;
	IC bool operator==(const CEdge& obj) const;
	IC const _edge_data_type& data() const;
	IC _edge_data_type& data();
};

template <typename _edge_weight_type, typename _vertex_type>
class CEdge<_edge_weight_type, _vertex_type, xr_empty> : 
	public CEdgeBase<_edge_weight_type,_vertex_type> {
private:
	typedef CEdgeBase<_edge_weight_type, _vertex_type> inherited;
	using _vertex_id_type = typename inherited::_vertex_id_type;

public:
	IC CEdge(const _edge_weight_type& weight, _vertex_type* vertex);
	IC bool operator==(const _vertex_id_type& vertex_id) const;
	IC bool operator==(const CEdge& obj) const;
};

#include "graph_edge_inline.h"
