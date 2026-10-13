////////////////////////////////////////////////////////////////////////////
//	Module 		: CVertex.h
//	Created 	: 14.01.2004
//  Modified 	: 19.02.2005
//	Author		: Dmitriy Iassenev
//	Description : Graph vertex base class template
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "object_broker.h"

template <typename _edge_weight_type, typename _vertex_type, typename _edge_data_type>
class CEdge;

// breaks the CVertex <-> CEdge instantiation cycle: matching the partial
// specialization does not require CEdge to be complete
template <typename _T>
struct _edge_weight_of
{
	typedef typename _T::_edge_weight_type type;
};

template <typename _E, typename _V, typename _D>
struct _edge_weight_of<CEdge<_E, _V, _D>>
{
	typedef _E type;
};

template <
	typename _data_type,
	typename _vertex_id_type_t,
	typename _graph_type
>
class CVertex
{
public:
	typedef _vertex_id_type_t _vertex_id_type;
	typedef typename _graph_type::CEdge _edge_type;
	typedef typename _edge_weight_of<_edge_type>::type _edge_weight_type;
	typedef xr_vector<_edge_type> EDGES;
	typedef xr_vector<CVertex*> VERTICES;

private:
	_vertex_id_type m_vertex_id;
	EDGES m_edges;
	_data_type m_data;
	// this container holds vertices, which has edges to us
	// this is needed for the fast vertex removal
	VERTICES m_vertices;
	// this counter is use for fast edge count computation in graph
	size_t* m_edge_count;

public:
	IC CVertex(const _data_type& data, const _vertex_id_type& vertex_id, size_t* edge_count);
	IC ~CVertex();
	IC bool operator==(const CVertex& obj) const;
	IC void add_edge(CVertex* vertex, const _edge_weight_type& edge_weight);
	IC void remove_edge(const _vertex_id_type& vertex_id);
	IC void on_edge_addition(CVertex* vertex);
	IC void on_edge_removal(const CVertex* vertex);
	IC const _vertex_id_type& vertex_id() const;
	IC const _data_type& data() const;
	IC _data_type& data();
	IC void data(const _data_type& data);
	IC const EDGES& edges() const;
	IC const _edge_type* edge(const _vertex_id_type& vertex_id) const;
	IC _edge_type* edge(const _vertex_id_type& vertex_id);
};

#include "graph_vertex_inline.h"
