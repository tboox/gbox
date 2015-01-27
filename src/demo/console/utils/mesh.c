/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t gb_demo_utils_mesh_test_splice()
{
    // trace
    tb_trace_i("==========================================================================");

    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        /* make a edge
         *
         *           lface
         *
         *         O -----> D
         *
         *           lface
         */
        gb_mesh_edge_ref_t edge = gb_mesh_edge_make(mesh);
        if (edge)
        {
            // save face name
            gb_mesh_face_data_set(mesh, gb_mesh_edge_lface(edge), "lface");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_org(edge), "org");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge), "dst");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("splice: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* splice
             * 
             *          -------
             *         |       |
             *         | rface |
             *         |       |
             *         O/D <---
             *
             *           lface
             * 
             */
            gb_mesh_edge_splice(mesh, edge, gb_mesh_edge_sym(edge));

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("splice: done");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* splice
             * 
             *           lface
             *
             *         O -----> D
             *
             *           lface
             * 
             */
            gb_mesh_edge_splice(mesh, edge, gb_mesh_edge_sym(edge));

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("splice: done");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif
        }

        // exit mesh
        gb_mesh_exit(mesh);
    }
}
static tb_void_t gb_demo_utils_mesh_test_radiation()
{
    // trace
    tb_trace_i("==========================================================================");

    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a edge
        gb_mesh_edge_ref_t edge1 = gb_mesh_edge_make(mesh);
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
            gb_mesh_edge_ref_t edge2 = gb_mesh_edge_append(mesh, gb_mesh_edge_sym(edge1));
            gb_mesh_edge_ref_t edge3 = gb_mesh_edge_append(mesh, gb_mesh_edge_sym(edge1));
            gb_mesh_edge_ref_t edge4 = gb_mesh_edge_append(mesh, gb_mesh_edge_sym(edge1));

            // save edge name
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");
            gb_mesh_edge_data_set(mesh, edge4, "e4");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_org(edge1), "v0");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge1), "v1");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge2), "v2");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge3), "v3");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge4), "v4");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("radiation: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* remove one 
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
            gb_mesh_edge_remove(mesh, edge1);

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("radiation: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // remove all 
            gb_mesh_edge_remove(mesh, edge2);
            gb_mesh_edge_remove(mesh, edge3);
            gb_mesh_edge_remove(mesh, edge4);

            // check
            tb_assert_abort(gb_mesh_is_empty(mesh));
        }

        // exit mesh
        gb_mesh_exit(mesh);
    }
}
static tb_void_t gb_demo_utils_mesh_test_quadrangle()
{
    // trace
    tb_trace_i("==========================================================================");

    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a counter-clockwise self-loop edge
        gb_mesh_edge_ref_t edge0 = gb_mesh_edge_make_loop(mesh, tb_true);
        if (edge0)
        {
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
            gb_mesh_edge_ref_t edge1 = gb_mesh_edge_insert(mesh, edge0, edge0);
            gb_mesh_edge_ref_t edge2 = gb_mesh_edge_insert(mesh, edge1, edge0);
            gb_mesh_edge_ref_t edge3 = gb_mesh_edge_insert(mesh, edge2, edge0);

            // save face name
            gb_mesh_face_data_set(mesh, gb_mesh_edge_lface(edge0), "lface");
            gb_mesh_face_data_set(mesh, gb_mesh_edge_rface(edge0), "rface");

            // save edge name
            gb_mesh_edge_data_set(mesh, edge0, "e0");
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge0), "v0");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge1), "v1");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge2), "v2");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge3), "v3");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("quadrangle: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            /* remove one
             *
             *                              v3
             *                   e1   .     |
             *                   .          | e3     rface
             *              .      lface    |
             *           v1 --------------> v2
             *                     e2

             */
            gb_mesh_edge_remove(mesh, edge0);

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("quadrangle: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // remove all
            gb_mesh_edge_remove(mesh, edge2);
            gb_mesh_edge_remove(mesh, edge3);
            gb_mesh_edge_remove(mesh, edge1);

            // check
            tb_assert_abort(gb_mesh_is_empty(mesh));
        }

    
        // exit mesh
        gb_mesh_exit(mesh);
    }
}
static tb_void_t gb_demo_utils_mesh_test_tetrahedron()
{
    // trace
    tb_trace_i("==========================================================================");

    // init mesh
    gb_mesh_ref_t mesh = gb_mesh_init(tb_item_func_str(tb_true), tb_item_func_str(tb_true), tb_item_func_str(tb_true));
    if (mesh)
    {
        // make a clockwise self-loop edge
        gb_mesh_edge_ref_t edge0 = gb_mesh_edge_make_loop(mesh, tb_false);
        if (edge0)
        {
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
            gb_mesh_edge_ref_t edge1 = gb_mesh_edge_insert(mesh, edge0, edge0);
            gb_mesh_edge_ref_t edge2 = gb_mesh_edge_insert(mesh, edge1, edge0);
            gb_mesh_edge_ref_t edge3 = gb_mesh_edge_insert(mesh, edge2, edge0);

            // save face name
            gb_mesh_face_data_set(mesh, gb_mesh_edge_lface(edge0), "lface");
            gb_mesh_face_data_set(mesh, gb_mesh_edge_rface(edge0), "rface");

            gb_mesh_edge_ref_t edge4 = gb_mesh_edge_connect(mesh, edge1, edge0);
            gb_mesh_edge_ref_t edge5 = gb_mesh_edge_connect(mesh, gb_mesh_edge_sym(edge3), gb_mesh_edge_sym(edge0));

            // save face name
            gb_mesh_face_data_set(mesh, gb_mesh_edge_lface(edge4), "face0");
            gb_mesh_face_data_set(mesh, gb_mesh_edge_lface(edge5), "face1");

            // save edge name
            gb_mesh_edge_data_set(mesh, edge0, "e0");
            gb_mesh_edge_data_set(mesh, edge1, "e1");
            gb_mesh_edge_data_set(mesh, edge2, "e2");
            gb_mesh_edge_data_set(mesh, edge3, "e3");
            gb_mesh_edge_data_set(mesh, edge4, "e4");
            gb_mesh_edge_data_set(mesh, edge5, "e5");

            // save vertex name
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge0), "v0");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge1), "v1");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge2), "v2");
            gb_mesh_vertex_data_set(mesh, gb_mesh_edge_dst(edge3), "v3");

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("tetrahedron: make");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // delete two
            gb_mesh_edge_delete(mesh, edge4);
            gb_mesh_edge_delete(mesh, edge5);

#ifdef __gb_debug__
            // trace
            tb_trace_i("");
            tb_trace_i("tetrahedron: kill");

            // check mesh
            gb_mesh_check(mesh);
        
            // dump mesh
            gb_mesh_dump(mesh);
#endif

            // remove all
            gb_mesh_edge_remove(mesh, edge2);
            gb_mesh_edge_remove(mesh, edge3);
            gb_mesh_edge_remove(mesh, edge0);
            gb_mesh_edge_remove(mesh, edge1);

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
    // test splice
    gb_demo_utils_mesh_test_splice();

    // test radiation
    gb_demo_utils_mesh_test_radiation();

    // test quadrangle
    gb_demo_utils_mesh_test_quadrangle();

    // test tetrahedron
    gb_demo_utils_mesh_test_tetrahedron();

    return 0;
}
