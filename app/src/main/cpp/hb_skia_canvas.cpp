//
// Created by Amine on 2023-07-12.
//

#include <hb.h>
#include <hb-machinery.hh>

#include "SkPath.h"
#include "SkCanvas.h"

#include "hb_skia_canvas.h"

static void
hb_skia_canvas_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
                        void *data,
                        hb_draw_state_t *st,
                        float to_x, float to_y,
                        void *user_data HB_UNUSED)
{
    SkPath *c = (SkPath *) data;

    c->moveTo (to_x, to_y);
}

static void
hb_skia_canvas_line_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
                        void *data,
                        hb_draw_state_t *st,
                        float to_x, float to_y,
                        void *user_data HB_UNUSED)
{
    SkPath *c = (SkPath *) data;

    c->lineTo(to_x, to_y);
}

static void
hb_skia_canvas_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
                             void *data,
                             hb_draw_state_t *st,
                             float control_x, float control_y,
                             float to_x, float to_y,
                             void *user_data HB_UNUSED)
{
    SkPath *c = (SkPath *) data;

    c->quadTo(control_x, control_y,to_x, to_y);
}

static void
hb_skia_canvas_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
                         void *data,
                         hb_draw_state_t *st,
                         float control1_x, float control1_y,
                         float control2_x, float control2_y,
                         float to_x, float to_y,
                         void *user_data HB_UNUSED)
{
    SkPath *c = (SkPath *) data;

    c->cubicTo(control1_x, control1_y,control2_x,control2_y,to_x, to_y);
}

static void
hb_skia_canvas_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
                           void *data,
                           hb_draw_state_t *st,
                           void *user_data HB_UNUSED)
{
    SkPath *c = (SkPath *) data;

    c->close();
}

static inline void free_static_skia_canvas_funcs();

static struct hb_skia_canvas_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_skia_canvas_funcs_lazy_loader_t>
{
    static hb_draw_funcs_t *create ()
    {
        hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

        hb_draw_funcs_set_move_to_func (funcs, hb_skia_canvas_move_to, nullptr, nullptr);
        hb_draw_funcs_set_line_to_func (funcs, hb_skia_canvas_line_to, nullptr, nullptr);
        hb_draw_funcs_set_quadratic_to_func (funcs, hb_skia_canvas_quadratic_to, nullptr, nullptr);
        hb_draw_funcs_set_cubic_to_func (funcs, hb_skia_canvas_cubic_to, nullptr, nullptr);
        hb_draw_funcs_set_close_path_func (funcs, hb_skia_canvas_close_path, nullptr, nullptr);

        hb_draw_funcs_make_immutable (funcs);

        hb_atexit (free_static_skia_canvas_funcs);

        return funcs;
    }
} static_skia_canvas_funcs;

static inline
void free_static_skia_canvas_funcs ()
{
    static_skia_canvas_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_skia_draw_get_funcs ()
{
    return static_skia_canvas_funcs.get_unconst ();
}

static void
hb_skia_push_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
                          void *paint_data,
                          hb_codepoint_t glyph,
                          hb_font_t *font,
                          void *user_data HB_UNUSED)
{
    skia_context_t *c = (skia_context_t *) paint_data;

    SkPath path;
    hb_font_draw_glyph (font, glyph, hb_skia_draw_get_funcs (), &path);
    c->path = path;

}
static void
hb_skia_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
                      void *paint_data,
                      hb_bool_t use_foreground,
                      hb_color_t color,
                      void *user_data HB_UNUSED)
{
    skia_context_t *c = (skia_context_t *) paint_data;
    //if(use_foreground){
    //    c->paint->setColor(SK_ColorBLACK);
    //}else{
        c->paint->setColor(SkColorSetARGB(hb_color_get_alpha (color), hb_color_get_red (color), hb_color_get_green (color), hb_color_get_blue (color)));
    //}

    c->canvas->drawPath(c->path, *c->paint);

}
static void
hb_skia_pop_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
                   void *paint_data,
                   void *user_data HB_UNUSED)
{

}

static inline void free_static_skia_paint_funcs ();

static struct hb_skia_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_skia_paint_funcs_lazy_loader_t>
{
    static hb_paint_funcs_t *create ()
    {
        hb_paint_funcs_t *paint_funcs = hb_paint_funcs_create ();


        hb_paint_funcs_set_push_clip_glyph_func (paint_funcs, hb_skia_push_clip_glyph, nullptr, nullptr);
        hb_paint_funcs_set_color_func (paint_funcs, hb_skia_paint_color, nullptr, nullptr);
        hb_paint_funcs_set_pop_clip_func (paint_funcs, hb_skia_pop_clip, nullptr, nullptr);

        hb_paint_funcs_make_immutable (paint_funcs);

        hb_atexit (free_static_skia_paint_funcs);

        return paint_funcs;
    }
} static_cairo_paint_funcs;

static inline
void free_static_skia_paint_funcs ()
{
    static_cairo_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_skia_paint_get_funcs ()
{
    return static_cairo_paint_funcs.get_unconst ();
}

void hb_skia_paint_glyph (hb_font_t *font,
hb_codepoint_t glyph, void *paint_data,
unsigned int palette_index,
        hb_color_t foreground)
{
hb_font_paint_glyph (font, glyph, hb_skia_paint_get_funcs(), paint_data, palette_index, foreground);
}

void hb_skia_render_glyph (hb_font_t *font, hb_codepoint_t glyph, void *draw_data){
    hb_font_draw_glyph (font,glyph,hb_skia_draw_get_funcs(),draw_data);
}


