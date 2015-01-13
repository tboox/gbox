/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
// the vertex
typedef struct __gb_mesh_demo_vertex_t
{
    // the id
    tb_size_t           id;

    // the point
    gb_point_t          point;

}gb_mesh_demo_vertex_t, *gb_mesh_demo_vertex_ref_t;

// the edge
typedef struct __gb_mesh_demo_edge_t
{
    // the id
    tb_size_t           id;

}gb_mesh_demo_edge_t, *gb_mesh_demo_edge_ref_t;

// the face
typedef struct __gb_mesh_demo_face_t
{
    // the id
    tb_size_t           id;

}gb_mesh_demo_face_t, *gb_mesh_demo_face_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t gb_mesh_demo_edge_exit(tb_item_func_t* func, tb_pointer_t buff)
{
    // the edge
    gb_mesh_demo_edge_ref_t edge = (gb_mesh_demo_edge_ref_t)buff;
    tb_assert_and_check_return(edge);

    // trace
    tb_trace_i("edge: exit: id: %lu", edge->id);
}
static tb_void_t gb_mesh_demo_face_exit(tb_item_func_t* func, tb_pointer_t buff)
{
    // the face
    gb_mesh_demo_face_ref_t face = (gb_mesh_demo_face_ref_t)buff;
    tb_assert_and_check_return(face);

    // trace
    tb_trace_i("face: exit: id: %lu", face->id);
}
static tb_void_t gb_mesh_demo_vertex_exit(tb_item_func_t* func, tb_pointer_t buff)
{
    // the vertex
    gb_mesh_demo_vertex_ref_t vertex = (gb_mesh_demo_vertex_ref_t)buff;
    tb_assert_and_check_return(vertex);

    // trace
    tb_trace_i("vertex: exit: id: %lu %{point}", vertex->id, &vertex->point);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_utils_mesh_main(tb_int_t argc, tb_char_t** argv)
{
    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(  tb_item_func_mem(sizeof(gb_mesh_demo_edge_t),   gb_mesh_demo_edge_exit,     tb_null)
                                    ,   tb_item_func_mem(sizeof(gb_mesh_demo_face_t),   gb_mesh_demo_face_exit,     tb_null)
                                    ,   tb_item_func_mem(sizeof(gb_mesh_demo_vertex_t), gb_mesh_demo_vertex_exit,   tb_null));
    if (mesh)
    {
        // make a self-loop edge
        gb_mesh_edge_ref_t edge = gb_mesh_make_loop_edge(mesh);
        if (edge)
        {
            // the left and right face
            gb_mesh_face_ref_t lface = gb_mesh_edge_lface(edge);
            gb_mesh_face_ref_t rface = gb_mesh_edge_rface(edge);

            /* make a quadrangle
             *
             *           v0 --------------> v1
             *            |                 |
             *            |      rface      |      lface
             *            |                 |
             *           v3 <-------------- v2
             */
            gb_mesh_vertex_ref_t vertex0 = gb_mesh_edge_dst(edge);
            gb_mesh_vertex_ref_t vertex1 = gb_mesh_make_vertex_edge(mesh, vertex0, lface, rface, tb_null);
            gb_mesh_vertex_ref_t vertex2 = gb_mesh_make_vertex_edge(mesh, vertex1, lface, rface, tb_null);
            gb_mesh_vertex_ref_t vertex3 = gb_mesh_make_vertex_edge(mesh, vertex2, lface, rface, tb_null);

            // make a tetrahedron
            gb_mesh_make_face_edge(mesh, lface, vertex1, vertex3, tb_null);
            gb_mesh_make_face_edge(mesh, rface, vertex0, vertex2, tb_null);
        }

#if 0
        // done
        tb_for_all_if (gb_mesh_vertex_ref_t, vertex, gb_mesh_vertex_itor(mesh), vertex)
        {
            // the user vertex
            gb_mesh_demo_vertex_ref_t vertex_user = (gb_mesh_demo_vertex_ref_t)gb_mesh_vertex_priv(vertex);

            // trace
            tb_trace_i("vertex: id: %lu %{point}", vertex_user->id, &vertex_user->point);
        }
#endif

#ifdef __gb_debug__
        // check mesh
        gb_mesh_check(mesh);
    
        // dump mesh
        gb_mesh_dump(mesh);
#endif
    
        // exit mesh
        gb_mesh_exit(mesh);
    }
    return 0;
}
