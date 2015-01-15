/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_utils_mesh_main(tb_int_t argc, tb_char_t** argv)
{
    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a self-loop edge
        gb_mesh_edge_ref_t edge0 = gb_mesh_make_loop_edge(mesh, tb_true);
        gb_mesh_edge_ref_t edge1 = tb_null;
        gb_mesh_edge_ref_t edge2 = tb_null;
        gb_mesh_edge_ref_t edge3 = tb_null;
        if (edge0)
        {
            // the left and right face
            gb_mesh_face_ref_t lface = gb_mesh_edge_lface(edge0);
            gb_mesh_face_ref_t rface = gb_mesh_edge_rface(edge0);

            // save face name
            gb_mesh_face_data_set(mesh, lface, "lface");
            gb_mesh_face_data_set(mesh, rface, "rface");

            /* make a quadrangle
             *
             *                     e3
             *           v0 <-------------- v3
             *            |                 |
             *         e0 |      lface      | e2     rface
             *            |                 |
             *           v1 --------------> v2
             *                     e1
             */
            gb_mesh_vertex_ref_t vertex0 = gb_mesh_edge_dst(edge0);
            gb_mesh_vertex_ref_t vertex1 = gb_mesh_make_vertex_edge(mesh, vertex0, lface, rface, &edge1);
            gb_mesh_vertex_ref_t vertex2 = gb_mesh_make_vertex_edge(mesh, vertex1, lface, rface, &edge2);
            gb_mesh_vertex_ref_t vertex3 = gb_mesh_make_vertex_edge(mesh, vertex2, lface, rface, &edge3);

            // save edge name
            gb_mesh_edge_data_set(mesh, edge0, "e0");
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, vertex0, "v0");
            gb_mesh_vertex_data_set(mesh, vertex1, "v1");
            gb_mesh_vertex_data_set(mesh, vertex2, "v2");
            gb_mesh_vertex_data_set(mesh, vertex3, "v3");

#if 0
            // make a tetrahedron
            gb_mesh_make_face_edge(mesh, lface, vertex1, vertex3, tb_null);
            gb_mesh_make_face_edge(mesh, rface, vertex0, vertex2, tb_null);
#endif
        }

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
