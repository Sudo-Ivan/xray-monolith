////////////////////////////////////////////////////////////////////////////
//	Module 		: vertex_manager_hash_fixed.h
//	Created 	: 21.03.2002
//  Modified 	: 05.03.2004
//	Author		: Dmitriy Iassenev
//	Description : Hash fixed vertex manager
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data_storage_constructor.h"
#include "graph_engine_space.h"

class shared_str;

namespace hash_fixed_vertex_manager
{
	IC u32 to_u32(const GraphEngineSpace::CWorldState& other);
	// GCC: qualified lookup in hash_index() happens at the template
	// definition point, so the shared_str overload must be declared here
	IC u32 to_u32(const shared_str& other);
}

template <
	typename _path_id_type_t,
	typename _index_type_t,
	u32 hash_size,
	u32 fix_size
>
struct CVertexManagerHashFixed
{
	template <template <typename _T> class T1>
	struct VertexManager
	{
		template <typename T2>
		struct _vertex : public T1<T2>
		{
			typedef _index_type_t _index_type;
			_index_type _index;
			bool _opened;

			IC const _index_type& index() const
			{
				return (_index);
			}

			IC _index_type& index()
			{
				return (_index);
			}

			IC bool& opened()
			{
				return (_opened);
			}

			IC bool opened() const
			{
				return (_opened);
			}
		};
	};

	struct _default_data_storage
	{
		template <template <typename> class _vertex>
		struct CDataStorage
		{
		};
	};

	template <
		template <typename _T> class _vertex = CEmptyClassTemplate,
		template <typename _T1, typename _T2> class _index_vertex = CEmptyClassTemplate2,
		typename _data_storage = _default_data_storage
	>
	class CDataStorage : public _data_storage::template CDataStorage<VertexManager<_vertex>::template _vertex>
	{
	public:
		typedef typename _data_storage::template CDataStorage<
			VertexManager<
				_vertex
			>::template _vertex
		> inherited;
		typedef typename inherited::CGraphVertex CGraphVertex;
		typedef typename CGraphVertex::_index_type _index_type;

#pragma pack(push,1)
		template <typename _path_id_type_i>
		struct SGraphIndexVertex : public _index_vertex<CGraphVertex, SGraphIndexVertex<_path_id_type_i>>
		{
			CGraphVertex* m_vertex;
			SGraphIndexVertex* m_next;
			SGraphIndexVertex* m_prev;
			u32 m_hash;
			_path_id_type_i m_path_id;
		};
#pragma pack(pop)

		typedef _path_id_type_t _path_id_type;
		typedef SGraphIndexVertex<_path_id_type> CGraphIndexVertex;

	protected:
		_path_id_type m_current_path_id;
		CGraphIndexVertex* m_vertices;
		CGraphIndexVertex** m_hash;
		u32 m_vertex_count;

	public:
		IC CDataStorage(const u32 vertex_count);
		virtual ~CDataStorage();
		IC void init();
		IC bool is_opened(const CGraphVertex& vertex) const;
		IC bool is_visited(const _index_type& vertex_id) const;
		IC bool is_closed(const CGraphVertex& vertex) const;
		IC CGraphVertex& get_node(const _index_type& vertex_id) const;
		IC CGraphVertex& create_vertex(CGraphVertex& vertex, const _index_type& vertex_id);
		IC void add_opened(CGraphVertex& vertex);
		IC void add_closed(CGraphVertex& vertex);
		IC _path_id_type current_path_id() const;
		IC u32 hash_index(const _index_type& vertex_id) const;
	};
};

#include "vertex_manager_hash_fixed_inline.h"
