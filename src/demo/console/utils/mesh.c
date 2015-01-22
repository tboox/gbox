/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t gb_demo_utils_mesh_test_radiation()
{
    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a edge
        gb_mesh_edge_ref_t edge1 = gb_mesh_edge_make(mesh);
        gb_mesh_edge_ref_t edge2 = tb_null;
        gb_mesh_edge_ref_t edge3 = tb_null;
        gb_mesh_edge_ref_t edge4 = tb_null;
        if (edge1)
        {
            // the face
            gb_mesh_face_ref_t face = gb_mesh_edge_lface(edge1);
            tb_assert_abort(face == gb_mesh_edge_rface(edge1));

            // save face name
            gb_mesh_face_data_set(mesh, face, "face");

            /* make a radiation
             *
             *                    v4
             *                   / \
             *                    |
             *                    |
             *                    | e4       
             *                    |   
             *                    |
             *                    |
             * <---------------- v0 ---------------->
             * v1       e1        |        e3       v3
             *                    |
             *                    |   
             *                    | e2       
             *                    |
             *                    |
             *                   \ /
             *                    v2
             */    
            gb_mesh_vertex_ref_t vertex0 = gb_mesh_edge_org(edge1);
            gb_mesh_vertex_ref_t vertex1 = gb_mesh_edge_dst(edge1);
            gb_mesh_vertex_ref_t vertex2 = gb_mesh_edge_make_at_vertex(mesh, vertex0, face, face, &edge2);
            gb_mesh_vertex_ref_t vertex3 = gb_mesh_edge_make_at_vertex(mesh, vertex0, face, face, &edge3);
            gb_mesh_vertex_ref_t vertex4 = gb_mesh_edge_make_at_vertex(mesh, vertex0, face, face, &edge4);

            // save edge name
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");
            gb_mesh_edge_data_set(mesh, edge4, "e4");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, vertex0, "v0");
            gb_mesh_vertex_data_set(mesh, vertex1, "v1");
            gb_mesh_vertex_data_set(mesh, vertex2, "v2");
            gb_mesh_vertex_data_set(mesh, vertex3, "v3");
            gb_mesh_vertex_data_set(mesh, vertex4, "v4");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("radiation: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* kill one edge
             *
             *                    v4
             *                   / \
             *                    |
             *                    |
             *                    | e4       
             *                    |   
             *                    |
             *                    |
             *                   v0 ---------------->
             *                    |        e3       v3
             *                    |
             *                    |   
             *                    | e2       
             *                    |
             *                    |
             *                   \ /
             *                    v2
             */    
            gb_mesh_edge_kill_at_vertex(mesh, edge1);

#ifdef __gb_debug__
            // trace
            tb_trace_i("radiation: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // kill all edges
            gb_mesh_edge_kill_at_vertex(mesh, edge2);
            gb_mesh_edge_kill_at_vertex(mesh, edge3);
            gb_mesh_edge_kill_at_vertex(mesh, edge4);

            // check
            tb_assert_abort(gb_mesh_is_empty(mesh));
        }

        // exit mesh
        gb_mesh_exit(mesh);
    }
}
static tb_void_t gb_demo_utils_mesh_test_quadrangle()
{
    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a counter-clockwise self-loop edge
        gb_mesh_edge_ref_t edge0 = gb_mesh_edge_make_loop(mesh, tb_true);
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
             *                     e0
             *           v0 <-------------- v3
             *            |                 |
             *         e1 |      lface      | e3     rface
             *            |                 |
             *           v1 --------------> v2
             *                     e2
             *
             */
            gb_mesh_vertex_ref_t vertex0 = gb_mesh_edge_dst(edge0);
            gb_mesh_vertex_ref_t vertex1 = gb_mesh_edge_make_at_vertex(mesh, vertex0, lface, rface, &edge1);
            gb_mesh_vertex_ref_t vertex2 = gb_mesh_edge_make_at_vertex(mesh, vertex1, lface, rface, &edge2);
            gb_mesh_vertex_ref_t vertex3 = gb_mesh_edge_make_at_vertex(mesh, vertex2, lface, rface, &edge3);

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

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("quadrangle: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* kill one edge
             *
             *                              v3
             *                   e1   .     |
             *                   .          | e3     rface
             *              .      lface    |
             *           v1 --------------> v2
             *                     e2

             */
            gb_mesh_edge_kill_at_vertex(mesh, edge0);

#ifdef __gb_debug__
            // trace
            tb_trace_i("quadrangle: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // kill all edges
            gb_mesh_edge_kill_at_vertex(mesh, edge2);
            gb_mesh_edge_kill_at_vertex(mesh, edge3);
            gb_mesh_edge_kill_at_vertex(mesh, edge1);

            // check
            tb_assert_abort(gb_mesh_is_empty(mesh));
        }

    
        // exit mesh
        gb_mesh_exit(mesh);
    }
}
static tb_void_t gb_demo_utils_mesh_test_tetrahedron()
{
    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a clockwise self-loop edge
        gb_mesh_edge_ref_t edge0 = gb_mesh_edge_make_loop(mesh, tb_false);
        gb_mesh_edge_ref_t edge1 = tb_null;
        gb_mesh_edge_ref_t edge2 = tb_null;
        gb_mesh_edge_ref_t edge3 = tb_null;
        gb_mesh_edge_ref_t edge4 = tb_null;
        gb_mesh_edge_ref_t edge5 = tb_null;
        if (edge0)
        {
            // the left and right face
            gb_mesh_face_ref_t lface = gb_mesh_edge_lface(edge0);
            gb_mesh_face_ref_t rface = gb_mesh_edge_rface(edge0);

            // save face name
            gb_mesh_face_data_set(mesh, lface, "lface");
            gb_mesh_face_data_set(mesh, rface, "rface");

            /* make a tetrahedron
             *
             *                     e1
             *           v0 --------------> v1-----------------
             *            | .         rface |                  |
             *         e0 |        .        | e2     lface     |
             *            | face1     e5 .  |                  | e4
             *           v3 <-------------- v2                 | 
             *            |        e3                          | 
             *            |                                    |   face0
             *             <-----------------------------------
             *
             */
            gb_mesh_vertex_ref_t    vertex0 = gb_mesh_edge_dst(edge0);
            gb_mesh_vertex_ref_t    vertex1 = gb_mesh_edge_make_at_vertex(mesh, vertex0, lface, rface, &edge1);
            gb_mesh_vertex_ref_t    vertex2 = gb_mesh_edge_make_at_vertex(mesh, vertex1, lface, rface, &edge2);
            gb_mesh_vertex_ref_t    vertex3 = gb_mesh_edge_make_at_vertex(mesh, vertex2, lface, rface, &edge3);

            gb_mesh_face_ref_t      face0 = gb_mesh_edge_make_at_face(mesh, lface, vertex1, vertex3, &edge4);
            gb_mesh_face_ref_t      face1 = gb_mesh_edge_make_at_face(mesh, rface, vertex0, vertex2, &edge5);

            // save edge name
            gb_mesh_edge_data_set(mesh, edge0, "e0");
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");
            gb_mesh_edge_data_set(mesh, edge4, "e4");
            gb_mesh_edge_data_set(mesh, edge5, "e5");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, vertex0, "v0");
            gb_mesh_vertex_data_set(mesh, vertex1, "v1");
            gb_mesh_vertex_data_set(mesh, vertex2, "v2");
            gb_mesh_vertex_data_set(mesh, vertex3, "v3");

            // save face name
            gb_mesh_face_data_set(mesh, face0, "f0");
            gb_mesh_face_data_set(mesh, face1, "f1");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("tetrahedron: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // kill two edges
            gb_mesh_edge_kill_at_face(mesh, edge4);
            gb_mesh_edge_kill_at_face(mesh, edge5);

#ifdef __gb_debug__
            // trace
            tb_trace_i("tetrahedron: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // kill all edges
            gb_mesh_edge_kill_at_vertex(mesh, edge2);
            gb_mesh_edge_kill_at_vertex(mesh, edge3);
            gb_mesh_edge_kill_at_vertex(mesh, edge0);
            gb_mesh_edge_kill_at_vertex(mesh, edge1);

            // check
            tb_assert_abort(gb_mesh_is_empty(mesh));
        }
    
        // exit mesh
        gb_mesh_exit(mesh);
    }
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t gb_demo_utils_mesh_main(tb_int_t argc, tb_char_t** argv)
{
    // test radiation
    gb_demo_utils_mesh_test_radiation();

    // test quadrangle
    gb_demo_utils_mesh_test_quadrangle();

    // test tetrahedron
    gb_demo_utils_mesh_test_tetrahedron();

    return 0;
}
