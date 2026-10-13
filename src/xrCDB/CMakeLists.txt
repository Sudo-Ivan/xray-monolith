# xrCDB - collision database (OPCODE)
set(XRCDB_SOURCES
    Frustum.cpp
    ISpatial.cpp
    ISpatial_q_box.cpp
    ISpatial_q_frustum.cpp
    ISpatial_q_ray.cpp
    ISpatial_verify.cpp
    OPC_AABBCollider.cpp
    OPC_AABB.cpp
    OPC_AABBTree.cpp
    OPC_Collider.cpp
    OPC_Common.cpp
    OPC_Container.cpp
    OPC_Matrix3x3.cpp
    OPC_Matrix4x4.cpp
    OPC_Model.cpp
    OPC_OBBCollider.cpp
    OPC_OBB.cpp
    Opcode.cpp
    OPC_OptimizedTree.cpp
    OPC_Plane.cpp
    OPC_PlanesCollider.cpp
    OPC_Point.cpp
    OPC_RayCollider.cpp
    OPC_Ray.cpp
    OPC_SphereCollider.cpp
    OPC_TreeBuilders.cpp
    OPC_TreeCollider.cpp
    OPC_Triangle.cpp
    OPC_VolumeCollider.cpp
    StdAfx.cpp
    xr_area.cpp
    xr_area_query.cpp
    xr_area_raypick.cpp
    xrCDB_box.cpp
    xrCDB_Collector.cpp
    xrCDB.cpp
    xrCDB_frustum.cpp
    xrCDB_ray.cpp
    xrXRC.cpp
)

add_library(xrcdb STATIC ${XRCDB_SOURCES})
target_include_directories(xrcdb PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrCDB"
)
target_link_libraries(xrcdb PUBLIC xrcore)
