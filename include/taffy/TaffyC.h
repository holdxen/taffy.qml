#pragma once

/* C ABI for the Rust taffy engine. No cxx — names are prefixed taffy_* . */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TaffyCPoint {
    double x;
    double y;
} TaffyCPoint;

typedef struct TaffyCSize {
    double width;
    double height;
} TaffyCSize;

typedef struct TaffyCRect {
    double left;
    double right;
    double top;
    double bottom;
} TaffyCRect;

typedef struct TaffyCLayout {
    uint32_t order;
    TaffyCPoint location;
    TaffyCSize size;
    TaffyCRect border;
    TaffyCRect padding;
    TaffyCRect margin;
    TaffyCSize scrollbar_size;
} TaffyCLayout;

typedef struct TaffyCLine {
    const char* start;
    const char* end;
} TaffyCLine;

typedef struct TaffyCStyle {
    const char* display;
    bool item_is_table;
    bool item_is_replaced;
    bool item_is_compressible_replaced;
    const char* box_sizing;
    const char* direction;
    const char* overflow_x;
    const char* overflow_y;
    double scrollbar_width;
    uint32_t contain;
    const char* css_float;
    const char* clear;
    const char* position;

    const char* inset_left;
    const char* inset_right;
    const char* inset_top;
    const char* inset_bottom;

    const char* size_width;
    const char* size_height;
    const char* min_size_width;
    const char* min_size_height;
    const char* max_size_width;
    const char* max_size_height;

    double aspect_ratio;

    const char* margin_left;
    const char* margin_right;
    const char* margin_top;
    const char* margin_bottom;

    const char* padding_left;
    const char* padding_right;
    const char* padding_top;
    const char* padding_bottom;

    const char* border_left;
    const char* border_right;
    const char* border_top;
    const char* border_bottom;

    const char* align_items;
    const char* align_self;
    const char* justify_items;
    const char* justify_self;
    const char* align_content;
    const char* justify_content;

    const char* gap_width;
    const char* gap_height;

    const char* text_align;
    const char* flex_direction;
    const char* flex_wrap;
    uint32_t flex_line_count;
    const char* flex_basis;
    double flex_grow;
    double flex_shrink;

    const char* grid_auto_flow;
    const char* grid_template_rows;
    const char* grid_template_columns;
    const char* grid_auto_rows;
    const char* grid_auto_columns;
    const char* grid_template_areas;

    const char* const* grid_template_row_names;
    size_t grid_template_row_names_len;
    const char* const* grid_template_column_names;
    size_t grid_template_column_names_len;

    TaffyCLine grid_row;
    TaffyCLine grid_column;

    double measure_width;
    double measure_height;
} TaffyCStyle;

typedef struct TaffyEngine TaffyEngine;

TaffyEngine* taffy_engine_new(void);
void taffy_engine_delete(TaffyEngine* engine);

uint32_t taffy_engine_add_node(TaffyEngine* engine, const TaffyCStyle* style);
int taffy_engine_add_child(TaffyEngine* engine, uint32_t parent, uint32_t child);
int taffy_engine_set_style(TaffyEngine* engine, uint32_t node, const TaffyCStyle* style);
int taffy_engine_compute(TaffyEngine* engine, const char* width_css, const char* height_css);
int taffy_engine_layout(const TaffyEngine* engine, uint32_t node, TaffyCLayout* out);
void taffy_engine_clear(TaffyEngine* engine);

#ifdef __cplusplus
}
#endif
