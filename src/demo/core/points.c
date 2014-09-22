/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "points"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "points.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_void_t gb_demo_points_init(gb_window_ref_t window)
{
}
tb_void_t gb_demo_points_exit(gb_window_ref_t window)
{
}
tb_void_t gb_demo_points_draw(gb_window_ref_t window, gb_canvas_ref_t canvas)
{
    // make points
    gb_point_t points[] = 
    {
        { gb_long_to_float(-100),   gb_long_to_float(-100)  }
    ,   { gb_long_to_float(100),    gb_long_to_float(100)   }
    ,   { gb_long_to_float(-100),   gb_long_to_float(100)   }
    ,   { gb_long_to_float(100),    gb_long_to_float(-100)  }
    ,   { gb_long_to_float(0),      gb_long_to_float(100)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(-100)  }
    ,   { gb_long_to_float(100),    gb_long_to_float(0)     }
    ,   { gb_long_to_float(-100),   gb_long_to_float(0)     }

    ,   { gb_long_to_float(-90),    gb_long_to_float(-90)   }
    ,   { gb_long_to_float(90),     gb_long_to_float(90)    }
    ,   { gb_long_to_float(-90),    gb_long_to_float(90)    }
    ,   { gb_long_to_float(90),     gb_long_to_float(-90)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(90)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-90)   }
    ,   { gb_long_to_float(90),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-90),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-80),    gb_long_to_float(-80)   }
    ,   { gb_long_to_float(80),     gb_long_to_float(80)    }
    ,   { gb_long_to_float(-80),    gb_long_to_float(80)    }
    ,   { gb_long_to_float(80),     gb_long_to_float(-80)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(80)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-80)   }
    ,   { gb_long_to_float(80),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-80),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-70),    gb_long_to_float(-70)   }
    ,   { gb_long_to_float(70),     gb_long_to_float(70)    }
    ,   { gb_long_to_float(-70),    gb_long_to_float(70)    }
    ,   { gb_long_to_float(70),     gb_long_to_float(-70)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(70)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-70)   }
    ,   { gb_long_to_float(70),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-70),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-60),    gb_long_to_float(-60)   }
    ,   { gb_long_to_float(60),     gb_long_to_float(60)    }
    ,   { gb_long_to_float(-60),    gb_long_to_float(60)    }
    ,   { gb_long_to_float(60),     gb_long_to_float(-60)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(60)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-60)   }
    ,   { gb_long_to_float(60),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-60),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-50),    gb_long_to_float(-50)   }
    ,   { gb_long_to_float(50),     gb_long_to_float(50)    }
    ,   { gb_long_to_float(-50),    gb_long_to_float(50)    }
    ,   { gb_long_to_float(50),     gb_long_to_float(-50)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(50)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-50)   }
    ,   { gb_long_to_float(50),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-50),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-40),    gb_long_to_float(-40)   }
    ,   { gb_long_to_float(40),     gb_long_to_float(40)    }
    ,   { gb_long_to_float(-40),    gb_long_to_float(40)    }
    ,   { gb_long_to_float(40),     gb_long_to_float(-40)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(40)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-40)   }
    ,   { gb_long_to_float(40),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-40),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-30),    gb_long_to_float(-30)   }
    ,   { gb_long_to_float(30),     gb_long_to_float(30)    }
    ,   { gb_long_to_float(-30),    gb_long_to_float(30)    }
    ,   { gb_long_to_float(30),     gb_long_to_float(-30)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(30)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-30)   }
    ,   { gb_long_to_float(30),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-30),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-20),    gb_long_to_float(-20)   }
    ,   { gb_long_to_float(20),     gb_long_to_float(20)    }
    ,   { gb_long_to_float(-20),    gb_long_to_float(20)    }
    ,   { gb_long_to_float(20),     gb_long_to_float(-20)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(20)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-20)   }
    ,   { gb_long_to_float(20),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-20),    gb_long_to_float(0)     }

    ,   { gb_long_to_float(-10),    gb_long_to_float(-10)   }
    ,   { gb_long_to_float(10),     gb_long_to_float(10)    }
    ,   { gb_long_to_float(-10),    gb_long_to_float(10)    }
    ,   { gb_long_to_float(10),     gb_long_to_float(-10)   }
    ,   { gb_long_to_float(0),      gb_long_to_float(10)    }
    ,   { gb_long_to_float(0),      gb_long_to_float(-10)   }
    ,   { gb_long_to_float(10),     gb_long_to_float(0)     }
    ,   { gb_long_to_float(-10),    gb_long_to_float(0)     }
    };

    // stroke
    gb_canvas_color_set(canvas, GB_COLOR_BLUE);
    gb_canvas_mode_set(canvas, GB_PAINT_MODE_STROKE);
    gb_canvas_draw_points(canvas, points, tb_arrayn(points));
}
tb_void_t gb_demo_points_event(gb_window_ref_t window, gb_event_ref_t event)
{
}

