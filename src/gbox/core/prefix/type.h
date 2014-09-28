/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        type.h
 * @ingroup     core
 *
 */
#ifndef GB_CORE_PREFIX_TYPE_H
#define GB_CORE_PREFIX_TYPE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../../prefix.h"
#include "pixfmt.h"
#include "float.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! @def gb_float_t
 *
 * the float type
 */
#ifdef GB_CONFIG_FLOAT_FIXED
typedef tb_fixed_t      gb_float_t;
#else
typedef tb_float_t      gb_float_t;
#endif

/// the pixel type
typedef tb_uint32_t     gb_pixel_t;

/*! @def gb_color_t 
 *
 * the color type
 *
 * @code
 * union __gb_c2p_t
 * {
 *      gb_color_t c;
 *      gb_pixel_t p;
 *
 * }c2p;
 * c2p.c = c;
 * return c2p.p;
 * @endcode
 *
 */
#ifdef TB_WORDS_BIGENDIAN
typedef struct __gb_color_t
{
    tb_byte_t           a;
    tb_byte_t           r;
    tb_byte_t           g;
    tb_byte_t           b;

}gb_color_t, *gb_color_ref_t;
#else
typedef struct __gb_color_t
{
    tb_byte_t           b;
    tb_byte_t           g;
    tb_byte_t           r;
    tb_byte_t           a;

}gb_color_t, *gb_color_ref_t;
#endif

/*! the matrix type
 *
 * <pre>
 * sx: the x-scale
 * sy: the y-scale
 * kx: the x-skew
 * ky: the y-skew
 * tx: the x-translate
 * ty: the y-translate
 *
 * x' = x * sx + y * kx + tx
 * y' = x * ky + y * sy + ty
 *
 * x'           sx kx tx    x * sx + y * kx + tx
 * y' = x y 1 * ky sy ty =  x * ky + y * sy + ty
 * 1             0  0  1                       1
 *
 * the following table describes how the members of the matrix are used for each type of
 * operation:
 *                    sx                kx               ky              sy                tx     ty
 * rotation           sx * cos          sx * -sin        sy * sin        sy * cos          0      0
 * scaling            sx                0                0               sy                0      0
 * translation        0                 0                0               0                 tx     ty
 * ...
 *
 * </pre>
 *
 */
typedef struct __gb_matrix_t
{
	gb_float_t 		    sx, kx, tx;
	gb_float_t 		    ky, sy, ty;

}gb_matrix_t, *gb_matrix_ref_t;

/// the point type
typedef struct __gb_point_t
{
    gb_float_t          x;
    gb_float_t          y;

}gb_point_t, *gb_point_ref_t;

/// the line type
typedef struct __gb_line_t
{
    gb_point_t          p0;
    gb_point_t          p1;

}gb_line_t, *gb_line_ref_t;

/// the triangle type
typedef struct __gb_triangle_t
{
    gb_point_t          p0;
    gb_point_t          p1;
    gb_point_t          p2;

}gb_triangle_t, *gb_triangle_ref_t;

/// the rect type
typedef struct __gb_rect_t
{
    gb_float_t          x;
    gb_float_t          y;
    gb_float_t          w;
    gb_float_t          h;

}gb_rect_t, *gb_rect_ref_t;

/// the rect corner enum
typedef enum __gb_rect_corner_e
{
    GB_RECT_CORNER_LT   = 0     //<! the left-top corner    
,   GB_RECT_CORNER_RT   = 1     //<! the right-top corner    
,   GB_RECT_CORNER_RB   = 2     //<! the right-bottom corner    
,   GB_RECT_CORNER_LB   = 3     //<! the left-bottom corner    
,   GB_RECT_CORNER_MAXN = 4     //<! the corner max count

}gb_rect_corner_e;

/// the round rect type
typedef struct __gb_round_rect_t
{
    // the bounds
    gb_rect_t           bounds;

    /*! the x-radius of the four corners
     *
     * <pre>
     *  
     *  lt                     rt
     *   --------------------->
     * /|\                     |
     *  |                      |
     *  |                      |
     *  |                      |
     *  |                     \|/
     *  <----------------------
     *  lb                     rb
     *
     * </pre>
     */
    gb_float_t          rx[GB_RECT_CORNER_MAXN];

    /*! the y-radius of the four corners
     * <pre>
     *  
     *  lt                     rt
     *   --------------------->
     * /|\                     |
     *  |                      |
     *  |                      |
     *  |                      |
     *  |                     \|/
     *  <----------------------
     *  lb                     rb
     *
     * </pre>
     */
    gb_float_t          ry[GB_RECT_CORNER_MAXN];

}gb_round_rect_t, *gb_round_rect_ref_t;

/// the circle type
typedef struct __gb_circle_t
{
    /// the center point
    gb_point_t          c;

    /// the radius
    gb_float_t          r;

}gb_circle_t, *gb_circle_ref_t;

/// the ellipse type
typedef struct __gb_ellipse_t
{
    /// the center point
    gb_point_t          c0;

    /// the x-radius 
    gb_float_t          rx;

    /// the y-radius
    gb_float_t          ry;

}gb_ellipse_t, *gb_ellipse_ref_t;

/// the arc type
typedef struct __gb_arc_t
{
    /// the center point
    gb_point_t          c0;

    /// the x-radius 
    gb_float_t          rx;

    /// the y-radius 
    gb_float_t          ry;

    /// the start angle
    gb_float_t          ab;

    /// the sweep angle, clockwise: > 0, counter-clockwise: < 0
    gb_float_t          an;

}gb_arc_t, *gb_arc_ref_t;

/*! the polygon type
 *
 * @code
    gb_point_t      points[] = {    {x0, y0}, {x1, y1}, {x2, y2}
                                ,   {x3, y3}, {x4, y4}, {x5, y5}, {x3, y3}};
    tb_size_t       counts[] = {3, 4, 0};
    gb_polygon_t    polygon = {points, counts}; 
 * @endcode
 */
typedef struct __gb_polygon_t
{
    /// the points
    gb_point_ref_t         points;

    /// the counts
    tb_uint16_t*        counts;

    /// is convex?
    tb_bool_t           convex;

}gb_polygon_t, *gb_polygon_ref_t;

/// the gradient type
typedef struct __gb_gradient_t
{
    /// the colors
    gb_color_t*         colors;

    /// the radios
    gb_float_t*         radios;

    /// the count
    tb_size_t           count;

}gb_gradient_t, *gb_gradient_ref_t;

/// the path ref type
typedef tb_iterator_ref_t gb_path_ref_t;

/// the shape type enum
typedef enum __gb_shape_type_e
{
    GB_SHAPE_TYPE_NONE          = 0x0000 //!< none
,   GB_SHAPE_TYPE_ARC           = 0x0001 //!< arc
,   GB_SHAPE_TYPE_PATH          = 0x0002 //!< path
,   GB_SHAPE_TYPE_LINE          = 0x0003 //!< line
,   GB_SHAPE_TYPE_RECT          = 0x0004 //!< rect
,   GB_SHAPE_TYPE_POINT         = 0x0005 //!< point
,   GB_SHAPE_TYPE_CIRCLE        = 0x0006 //!< circle
,   GB_SHAPE_TYPE_ELLIPSE       = 0x0007 //!< ellipse
,   GB_SHAPE_TYPE_POLYGON       = 0x0008 //!< polygon
,   GB_SHAPE_TYPE_TRIANGLE      = 0x0009 //!< triangle
,   GB_SHAPE_TYPE_ROUND_RECT    = 0x0010 //!< round rect

}gb_shape_type_e;

/// the shape type
typedef struct __gb_shape_t
{
    /// the type
    tb_size_t           type;

    /// the shape
    union
    {
        /// the arc
        gb_arc_t        arc;

        /// the path
        gb_path_ref_t   path;

        /// the line
        gb_line_t       line;

        /// the rect
        gb_rect_t       rect;

        /// the point
        gb_point_t      point;

        /// the circle
        gb_circle_t     circle;
    
        /// the ellipse
        gb_ellipse_t    ellipse;
 
        /// the polygon
        gb_polygon_t    polygon;
 
        /// the triangle
        gb_triangle_t   triangle;
 
        /// the round rect
        gb_round_rect_t round_rect;
    }u;

}gb_shape_t, *gb_shape_ref_t;

/// the device ref type
typedef struct{}*       gb_device_ref_t;

/// the canvas ref type
typedef struct{}*       gb_canvas_ref_t;

/// the paint ref type
typedef struct{}*       gb_paint_ref_t;

/// the bitmap ref type
typedef struct{}*       gb_bitmap_ref_t;

/// the shader ref type
typedef struct{}*       gb_shader_ref_t;

/// the clipper ref type
typedef struct{}*       gb_clipper_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * inline
 */
__tb_extern_c_enter__

/*! make point
 *
 * @param x                         the x
 * @param y                         the y
 *
 * @return                          the point
 */
static __tb_inline__ gb_point_t     gb_point_make(gb_float_t x, gb_float_t y)
{
    gb_point_t pt;

    pt.x = x;
    pt.y = y;

    return pt;
}

/*! make point with the integer value
 *
 * @param x                         the x
 * @param y                         the y
 *
 * @return                          the point
 */
static __tb_inline__ gb_point_t     gb_point_imake(tb_long_t x, tb_long_t y)
{
    gb_point_t pt;

    pt.x = gb_long_to_float(x);
    pt.y = gb_long_to_float(y);

    return pt;
}

/*! make rect
 *
 * @param x                         the x
 * @param y                         the y
 * @param w                         the width
 * @param h                         the height
 *
 * @return                          the rect
 */
static __tb_inline__ gb_rect_t      gb_rect_make(gb_float_t x, gb_float_t y, gb_float_t w, gb_float_t h)
{
    gb_rect_t r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    return r;
}

/*! make rect with the integer value
 *
 * @param x                         the x
 * @param y                         the y
 * @param w                         the width
 * @param h                         the height
 *
 * @return                          the rect
 */
static __tb_inline__ gb_rect_t      gb_rect_imake(tb_long_t x, tb_long_t y, tb_size_t w, tb_size_t h)
{
    gb_rect_t r;

    r.x = gb_long_to_float(x);
    r.y = gb_long_to_float(y);
    r.w = gb_long_to_float(w);
    r.h = gb_long_to_float(h);

    return r;
}

/*! make line
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param x1                        the x1
 * @param y1                        the y1
 *
 * @return                          the line
 */
static __tb_inline__ gb_line_t      gb_line_make(gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1)
{
    gb_line_t l;

    l.p0.x = x0;
    l.p0.y = y0;
    l.p1.x = x1;
    l.p1.y = y1;

    return l;
}

/*! make line with the integer value
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param x1                        the x1
 * @param y1                        the y1
 *
 * @return                          the line
 */
static __tb_inline__ gb_line_t      gb_line_imake(tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1)
{
    gb_line_t l;

    l.p0.x = gb_long_to_float(x0);
    l.p0.y = gb_long_to_float(y0);
    l.p1.x = gb_long_to_float(x1);
    l.p1.y = gb_long_to_float(y1);

    return l;
}

/*! make triangle
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param x1                        the x1
 * @param y1                        the y1
 * @param x2                        the x2
 * @param y2                        the y2
 *
 * @return                          the triangle
 */
static __tb_inline__ gb_triangle_t  gb_triangle_make(gb_float_t x0, gb_float_t y0, gb_float_t x1, gb_float_t y1, gb_float_t x2, gb_float_t y2)
{
    gb_triangle_t t;

    t.p0.x = x0;
    t.p0.y = y0;
    t.p1.x = x1;
    t.p1.y = y1;
    t.p2.x = x2;
    t.p2.y = y2;

    return t;
}

/*! make triangle with the integer value
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param x1                        the x1
 * @param y1                        the y1
 * @param x2                        the x2
 * @param y2                        the y2
 *
 * @return                          the triangle
 */
static __tb_inline__ gb_triangle_t  gb_triangle_imake(tb_long_t x0, tb_long_t y0, tb_long_t x1, tb_long_t y1, tb_long_t x2, tb_long_t y2)
{
    gb_triangle_t t;

    t.p0.x = gb_long_to_float(x0);
    t.p0.y = gb_long_to_float(y0);
    t.p1.x = gb_long_to_float(x1);
    t.p1.y = gb_long_to_float(y1);
    t.p2.x = gb_long_to_float(x2);
    t.p2.y = gb_long_to_float(y2);

    return t;
}

/*! make circle
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param r                         the radius
 *
 * @return                          the circle
 */
static __tb_inline__ gb_circle_t    gb_circle_make(gb_float_t x0, gb_float_t y0, gb_float_t r)
{
    gb_circle_t c;

    c.c.x   = x0;
    c.c.y   = y0;
    c.r     = r;

    return c;
}

/*! make circle with the integer value
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param r                         the radius
 *
 * @return                          the circle
 */
static __tb_inline__ gb_circle_t    gb_circle_imake(tb_long_t x0, tb_long_t y0, tb_size_t r)
{
    gb_circle_t c;

    c.c.x   = gb_long_to_float(x0);
    c.c.y   = gb_long_to_float(y0);
    c.r     = gb_long_to_float(r);

    return c;
}

/*! make ellipse
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param rx                        the x-radius
 * @param ry                        the y-radius
 *
 * @return                          the ellipse
 */
static __tb_inline__ gb_ellipse_t   gb_ellipse_make(gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry)
{
    gb_ellipse_t e;

    e.c0.x  = x0;
    e.c0.y  = y0;
    e.rx    = rx;
    e.ry    = ry;

    return e;
}

/*! make ellipse with the integer value
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param rx                        the x-radius
 * @param ry                        the y-radius
 *
 * @return                          the ellipse
 */
static __tb_inline__ gb_ellipse_t   gb_ellipse_imake(tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry)
{
    gb_ellipse_t e;

    e.c0.x  = gb_long_to_float(x0);
    e.c0.y  = gb_long_to_float(y0);
    e.rx    = gb_long_to_float(rx);
    e.ry    = gb_long_to_float(ry);

    return e;
}

/*! make arc
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param rx                        the x-radius
 * @param ry                        the y-radius
 * @param ab                        the start angle
 * @param an                        the sweep angle, 0 - 360
 *
 * @return                          the arc
 */
static __tb_inline__ gb_arc_t       gb_arc_make(gb_float_t x0, gb_float_t y0, gb_float_t rx, gb_float_t ry, gb_float_t ab, gb_float_t an)
{
    gb_arc_t a;

    a.c0.x  = x0;
    a.c0.y  = y0;
    a.rx    = rx;
    a.ry    = ry;
    a.ab    = ab;
    a.an    = an;

    return a;
}

/*! make arc with the integer value
 *
 * @param x0                        the x0
 * @param y0                        the y0
 * @param rx                        the x-radius
 * @param ry                        the y-radius
 * @param ab                        the start angle
 * @param an                        the sweep angle, 0 - 360
 *
 * @return                          the arc
 */
static __tb_inline__ gb_arc_t       gb_arc_imake(tb_long_t x0, tb_long_t y0, tb_size_t rx, tb_size_t ry, tb_long_t ab, tb_long_t an)
{
    gb_arc_t a;

    a.c0.x  = gb_long_to_float(x0);
    a.c0.y  = gb_long_to_float(y0);
    a.rx    = gb_long_to_float(rx);
    a.ry    = gb_long_to_float(ry);
    a.ab    = gb_long_to_float(ab);
    a.an    = gb_long_to_float(an);

    return a;
}


__tb_extern_c_leave__
#endif


