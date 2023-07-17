//
// Created by Amine Anane on 2023-07-12.
//


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>




#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkSurface.h"
#include "SkString.h"
#include "SkTime.h"
#include "SkFont.h"
#include "SkTextBlob.h"
#include "SkData.h"
#include "SkImage.h"
#include "SkStream.h"
#include "SkSurface.h"
#include "SkPath.h"
#include "hb_skia_canvas.h"

#include <hb.h>
#include <hb-buffer.hh>
#include <hb-font.hh>
#include "quran.h"
#include <string>
#include <sstream>
#include <regex>
#include <unordered_map>

enum class  LineType {
    Line = 0,
    Sura = 1,
    Bism = 2
};

enum class  JustType {
    just = 0,
    center = 1,
};
typedef struct
{
    std::string text;
    LineType line_type = LineType::Line;
    JustType just_type = JustType::just;
} quran_line;

static hb_face_t *face;
static std::vector<std::vector<quran_line>> pages;
static hb_language_t ar_language;
static unsigned int upem;
static hb_font_t *font;
static hb_draw_funcs_t * draw_funcs;
static hb_paint_funcs_t * paint_funcs;
static bool tajweed = true;
static bool changeSize = true;
static int tajweedcolorindex = 152;

static std::unordered_map<int, float> lineWidths =
{
    { 15 * 585 + 0, 0.81},
    { 15 * 592 + 1, 0.81},
    { 15 * 593 + 4, 0.63},
    { 15 * 599 + 9,0.63 },
    { 15 * 601 + 4, 0.63 },
    { 15 * 601 + 10, 0.9 },
    { 15 * 601 + 14, 0.53 },
    { 15 * 602 + 9, 0.66 },
    { 15 * 602 + 14, 0.60 },
    { 15 * 603 + 3, 0.55 },
    { 15 * 603 + 8, 0.55 },
    { 15 * 603 + 13, 0.675 },
    { 15 * 603 + 14, 0.5 },
};

static hb_feature_t features[]{{ HB_TAG('t', 'j', 'w', 'd'),tajweed,0,(uint)-1 }};

static int coords[2];

static void drawLine(quran_line& lineText, skia_context_t* context,double lineWidth, bool justify, double scale){

    unsigned count = 0;
    hb_glyph_info_t *glyph_info = nullptr;
    hb_glyph_position_t* glyph_pos = nullptr;
    int spaceCodePoint = 3;
    int currentLineWidth;
    double spaceWidth;


    auto canvas = context->canvas;

    hb_buffer_t * buffer = hb_buffer_create ();
    hb_buffer_set_direction(buffer, HB_DIRECTION_RTL);
    hb_buffer_set_script(buffer, HB_SCRIPT_ARABIC);
    hb_buffer_set_language(buffer, ar_language);

    hb_buffer_add_utf8(buffer, lineText.text.c_str(), lineText.text.size(), 0, lineText.text.size());

    if(justify && lineText.just_type == JustType::just){
        hb_buffer_set_justify(buffer, lineWidth);
    }
    //buffer->justContext = nullptr;
    hb_shape(font, buffer, features, 1);


    glyph_info = hb_buffer_get_glyph_infos (buffer, &count);
    glyph_pos = hb_buffer_get_glyph_positions(buffer, &count);

    int textWidth = 0;
    int nbSpaces = 0;
    currentLineWidth=0;

    for (int i = count - 1; i >= 0; i--){
        if(glyph_info[i].codepoint == spaceCodePoint){
            nbSpaces++;
        }else{
            textWidth+= glyph_pos[i].x_advance;
        }
        currentLineWidth+= glyph_pos[i].x_advance;
    }

    if(currentLineWidth > lineWidth){

    }

    bool applySpaceWidth = false;

    if(currentLineWidth > lineWidth){
        if(changeSize){
            double ratio = (double)lineWidth / currentLineWidth;
            canvas->scale(ratio,ratio);
            scale = scale / ratio;
            currentLineWidth = lineWidth;
            textWidth = textWidth * ratio;
        }
    }else if (textWidth < lineWidth){
        spaceWidth = (lineWidth - textWidth) / (double)nbSpaces;
        applySpaceWidth = true;
    }

    if(lineText.just_type == JustType::center){
        auto pad = (lineWidth - currentLineWidth) / 2;
        canvas->translate(-pad,0);
    }

    for (int i = count - 1; i >= 0; i--)
    {
        auto glyph_index = glyph_info[i].codepoint;
        bool extend = false;

        if(glyph_info[i].lefttatweel != 0 || glyph_info[i].righttatweel != 0){
            extend = true;

            coords[0] = roundf (glyph_info[i].lefttatweel * 16384.f);
            coords[1] = roundf (glyph_info[i].righttatweel * 16384.f);
            font->num_coords = 2;
            font->coords = &coords[0];
        }

        /*__android_log_print(2, "drawArabicText",
                            "\tGlyph%d = %d\tx_offset = %d\ty_offset = %d\tx_advance = %d\ty_advance = %d\n",
                            i, glyph_index,glyph_pos[i].x_offset, glyph_pos[i].y_offset,
                            glyph_pos[i].x_advance, glyph_pos[i].y_advance);*/

        if(glyph_info[i].codepoint == spaceCodePoint && lineText.just_type == JustType::just && applySpaceWidth){
            canvas->translate(-spaceWidth,0);
        }else{
            canvas->translate(-glyph_pos[i].x_advance,0);
        }

        //canvas->save();
        canvas->translate(glyph_pos[i].x_offset,glyph_pos[i].y_offset);


        //SkPath path;
        //hb_font_draw_glyph (l_font, glyph_index, draw_funcs, &path);
        ////hb_skia_render_glyph(l_font, glyph_index, &path);
        //canvas->drawPath(path, *context->paint);

        auto color = HB_COLOR (0, 0, 0, 255);
        if(tajweed && glyph_pos[i].lookup_index >= tajweedcolorindex){
            color = HB_COLOR ((glyph_pos[i].base_codepoint >> 8) & 0xff, (glyph_pos[i].base_codepoint >> 16) & 0xff, (glyph_pos[i].base_codepoint >> 24) & 0xff, 255);
        }
        hb_font_paint_glyph(font, glyph_index,paint_funcs, context, 0, color);

        canvas->translate(-glyph_pos[i].x_offset,-glyph_pos[i].y_offset);
        //canvas->restore();


        if(extend){
            font->num_coords = 0;
            font->coords = nullptr;
        }
    }

    hb_buffer_destroy (buffer);


}

extern "C"
JNIEXPORT void JNICALL
Java_org_digitalkhatt_mushaf_PageView_drawPage(JNIEnv *env, jobject thiz, jobject image,
											   jint page_index, jint xx_start, jint y_start,
											   jint inter_line, jint char_height,jboolean justify) {

    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    AndroidBitmap_getInfo(env, image, &dstInfo);
    AndroidBitmap_lockPixels(env, image, &dstPixels);

    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(dstInfo.width, dstInfo.height);

    auto canvas = SkCanvas::MakeRasterDirect(imageInfo, dstPixels, dstInfo.stride);

    canvas->drawColor(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorBLACK);
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);

    skia_context_t context{};
    context.canvas = canvas.get();
    context.paint = &paint;

    auto &pageText = pages[page_index];

    char_height = (dstInfo.width / 17 ) * 0.9;
    inter_line = dstInfo.height / 15;
    y_start = inter_line * 0.72;
    int x_padding = dstInfo.width / 42.5;

    double scale  = (double)char_height / upem;

    int x_start = dstInfo.width - x_padding;

    double pageWidth =  (dstInfo.width - 2*x_padding)/ scale;

    if(page_index == 0 || page_index == 1){
        y_start = y_start + 3.5 * inter_line;
    }

    for(int lineIndex = 0; lineIndex < pageText.size();lineIndex++){
        canvas->resetMatrix();

        auto lineWidth = pageWidth;

        auto specialWidth = lineWidths.find (page_index*15 + lineIndex);
        if(specialWidth != lineWidths.end()){
            lineWidth = pageWidth * specialWidth->second;
            float xxstart = (pageWidth - lineWidth)/2;
            canvas->translate(x_start - xxstart * scale,y_start + lineIndex * inter_line);
        }else{
            canvas->translate(x_start,y_start + lineIndex * inter_line);
        }
        canvas->scale(scale,-scale);

        auto& linetext = pageText[lineIndex];

        drawLine(linetext,&context,lineWidth,justify,scale);
    }

    // Unlock the dst's pixels
    AndroidBitmap_unlockPixels(env, image);

}
extern "C"
JNIEXPORT void JNICALL
Java_org_digitalkhatt_mushaf_MushafApp_nativeInit(JNIEnv *env, jobject thiz, jobject assetManager) {
    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);

    AAsset* fontFile = AAssetManager_open(manager, "fonts/digitalkhatt.otf", AASSET_MODE_BUFFER);
    off_t fontDataSize = AAsset_getLength(fontFile);

    char* fontData = new char[fontDataSize];

    AAsset_read(fontFile, fontData, (size_t) fontDataSize);
    AAsset_close(fontFile);

    auto blob = hb_blob_create_or_fail (fontData, fontDataSize, HB_MEMORY_MODE_WRITABLE, nullptr,nullptr);

    face = hb_face_create (blob, 0 /* first face */);
    hb_blob_destroy (blob);
    blob = nullptr;

    auto bism1 = "بِسْمِ ٱللَّهِ ٱلرَّحْمَٰنِ ٱلرَّحِيمِ";
    auto bism2 = "بِسْمِ ٱللَّهِ ٱلرَّحْمَٰنِ ٱلرَّحِيمِ";

    std::regex begin_sura("^(سُورَة.*)$");

    auto pageWidth = 17000;

    for(int pageIndex =0 ; pageIndex< 604; pageIndex++){
        auto&page = pages.emplace_back();
        auto pagetext = qurantext[pageIndex] + 1;
        std::stringstream ss(pagetext);
        quran_line line;
        int lineIndex = -1;
        while (std::getline(ss, line.text, '\n')) {
            lineIndex++;
            if(line.text == bism1 || line.text == bism2) {
                line.line_type = LineType::Bism;
                line.just_type = JustType::center;
            }else if (regex_match (line.text,begin_sura)){
                line.line_type = LineType::Sura;
                line.just_type = JustType::center;
            }else{
                line.line_type = LineType::Line;
                line.just_type = JustType::just;
            }
            if(pageIndex == 0 || pageIndex == 1){
                if (lineIndex > 0) {
                    line.just_type = JustType::just;
                    line.line_type = LineType::Line;
                    double ratio = pageIndex == 0 ? 0.9 : 0.9;
                    double diameter = pageWidth * ratio; // 0.9;
                    if (pageIndex == 0) {
                        diameter = pageWidth * ratio; // 0.9;
                    }
                    // 22.5 = 180 / 8
                    double startangle = pageIndex == 0 ? 30 : 30;
                    double endangle = 22.5;


                    double degree = (startangle + (lineIndex - 1) * (180 - (startangle + endangle)) / 6) * M_PI / 180;
                    double lineWidth = diameter * std::sin(degree);
                    lineWidths[15 * pageIndex + lineIndex ]= lineWidth/pageWidth;
                }
                else {
                    line.just_type = JustType::center;
                }

            }
            page.push_back(line);
        }
    }

    ar_language = hb_language_from_string("ar", strlen("ar"));

    upem = hb_face_get_upem(face);

    font = hb_font_create (face);
    hb_font_set_scale(font,upem,upem);

    draw_funcs = hb_skia_draw_get_funcs ();
    paint_funcs =  hb_skia_paint_get_funcs ();

}

extern "C"
JNIEXPORT void JNICALL
Java_org_digitalkhatt_mushaf_MushafApp_set_1tajweed(JNIEnv *env, jobject thiz, jboolean ptajweed) {
    tajweed = ptajweed;
    features[0].value = tajweed;

}