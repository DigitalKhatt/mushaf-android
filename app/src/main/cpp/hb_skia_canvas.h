//
// Created by Amine on 2023-07-12.
//

#ifndef MUSHAF_HB_SKIA_CANVAS_H
#define MUSHAF_HB_SKIA_CANVAS_H

#include <hb.h>

typedef struct
{
    SkCanvas *canvas;
    SkPath path;
    SkPaint * paint;
} skia_context_t;

void hb_skia_paint_glyph (hb_font_t *font,
                          hb_codepoint_t glyph, void *paint_data,
                          unsigned int palette_index,
                          hb_color_t foreground);

void hb_skia_render_glyph (hb_font_t *font, hb_codepoint_t glyph, void *draw_data);


hb_draw_funcs_t * hb_skia_draw_get_funcs ();
hb_paint_funcs_t * hb_skia_paint_get_funcs ();

#endif //MUSHAF_HB_SKIA_CANVAS_H
