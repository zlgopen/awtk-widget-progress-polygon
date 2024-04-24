/**
 * File:   progress_polygon.c
 * Author: AWTK Develop Team
 * Brief:  异形进度条。
 *
 * Copyright (c) 2024 - 2024 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-04-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "progress_polygon.h"



static uint32_t polygon_points_find(polygon_points_t* points, double value) {
  uint32_t i = 0;
  polygon_point_t* iter = NULL;
  return_value_if_fail(points != NULL, 0);

  for (i = 0; i < points->size; i++) {
    iter = points->points + i;
    if (iter->value >= value) {
      return i;
    }
  }

  return points->size;
}

ret_t polygon_points_init(polygon_points_t* arr, const char* data) {
  const char* p = data;
  polygon_point_t* iter = NULL;
  uint32_t n = tk_count_char(data, '(');
  return_value_if_fail(arr != NULL && data != NULL, RET_BAD_PARAMS);

  arr->size = 0;
  arr->capacity = n;
  arr->points = TKMEM_ZALLOCN(polygon_point_t, n);
  return_value_if_fail(arr->points != NULL, RET_OOM);

  do {
    p = tk_skip_to_chars(p, "(");
    if (p == NULL || *p == '\0') break;

    iter = arr->points + arr->size++;
    p = tk_skip_chars(p, " ");
    iter->value = tk_atof(p + 1);

    p = tk_skip_to_chars(p, ",");
    break_if_fail(*p == ',');
    p = tk_skip_chars(p, " ");
    iter->x1 = tk_atof(p + 1);

    p = tk_skip_to_chars(p + 1, ",");
    break_if_fail(*p == ',');
    p = tk_skip_chars(p, " ");
    iter->y1 = tk_atof(p + 1);

    p = tk_skip_to_chars(p + 1, ",");
    break_if_fail(*p == ',');
    p = tk_skip_chars(p, " ");
    iter->x2 = tk_atof(p + 1);

    p = tk_skip_to_chars(p + 1, ",");
    break_if_fail(*p == ',');
    p = tk_skip_chars(p, " ");
    iter->y2 = tk_atof(p + 1);

    p = tk_skip_to_chars(p + 1, ")");
    break_if_fail(*p == ')');
    assert(arr->size <= arr->capacity);
  } while (TRUE);

  return RET_OK;
}

ret_t polygon_points_deinit(polygon_points_t* points) {
  return_value_if_fail(points != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(points->points);
  memset(points, 0x00, sizeof(polygon_points_t));

  return RET_OK;
}

ret_t progress_polygon_set_value(widget_t* widget, double value) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL, RET_BAD_PARAMS);

  progress_polygon->value = value;

  return RET_OK;
}

ret_t progress_polygon_set_min(widget_t* widget, double min) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL, RET_BAD_PARAMS);

  progress_polygon->min = min;

  return RET_OK;
}

ret_t progress_polygon_set_max(widget_t* widget, double max) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL, RET_BAD_PARAMS);

  progress_polygon->max = max;

  return RET_OK;
}

ret_t progress_polygon_set_polygon(widget_t* widget, const char* polygon) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL, RET_BAD_PARAMS);

  progress_polygon->polygon = tk_str_copy(progress_polygon->polygon, polygon);
  polygon_points_deinit(&progress_polygon->points);
  polygon_points_init(&progress_polygon->points, polygon);

  return RET_OK;
}

static ret_t progress_polygon_get_prop(widget_t* widget, const char* name, value_t* v) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(PROGRESS_POLYGON_PROP_VALUE, name)) {
    value_set_double(v, progress_polygon->value);
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_MIN, name)) {
    value_set_double(v, progress_polygon->min);
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_MAX, name)) {
    value_set_double(v, progress_polygon->max);
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_POLYGON, name)) {
    value_set_str(v, progress_polygon->polygon);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_polygon_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(PROGRESS_POLYGON_PROP_VALUE, name)) {
    progress_polygon_set_value(widget, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_MIN, name)) {
    progress_polygon_set_min(widget, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_MAX, name)) {
    progress_polygon_set_max(widget, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(PROGRESS_POLYGON_PROP_POLYGON, name)) {
    progress_polygon_set_polygon(widget, value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_polygon_on_destroy(widget_t* widget) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && progress_polygon != NULL, RET_BAD_PARAMS);

  polygon_points_deinit(&progress_polygon->points);
  TKMEM_FREE(progress_polygon->polygon);

  return RET_OK;
}

static pointf_t progress_polygon_normalize_point(widget_t* widget, float x, float y) {
  pointf_t p = {0};
  return_value_if_fail(widget != NULL, p);
  p.x = x > 1 ? x : x * widget->w;
  p.y = y > 1 ? y : y * widget->h;

  return p;
}

static ret_t progress_polygon_move_to(widget_t* widget, vgcanvas_t* vg, float x, float y) {
  pointf_t point = {0};
  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  point = progress_polygon_normalize_point(widget, x, y);
  vgcanvas_move_to(vg, point.x, point.y);

  return RET_OK;
}

static ret_t progress_polygon_line_to(widget_t* widget, vgcanvas_t* vg, float x, float y) {
  pointf_t point = {0};
  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  point = progress_polygon_normalize_point(widget, x, y);
  vgcanvas_line_to(vg, point.x, point.y);

  return RET_OK;
}

static ret_t pogress_polygon_draw_border(widget_t* widget, vgcanvas_t* vg, color_t border_color,
                                         uint32_t line_width) {
  int32_t i = 0;
  polygon_point_t* iter = NULL;
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  vgcanvas_begin_path(vg);
  iter = progress_polygon->points.points;
  progress_polygon_move_to(widget, vg, iter->x1, iter->y1);
  for (i = 1; i < progress_polygon->points.size; i++) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x1, iter->y1);
  }

  for (i = progress_polygon->points.size - 1; i >= 0; i--) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x2, iter->y2);
  }

  vgcanvas_close_path(vg);
  vgcanvas_set_line_width(vg, line_width);
  vgcanvas_set_stroke_color(vg, border_color);
  vgcanvas_stroke(vg);

  return RET_OK;
}

static ret_t progress_polygon_fill(widget_t* widget, vgcanvas_t* vg, color_t color,
                                   const char* image) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  if (image != NULL) {
    bitmap_t img;
    if (widget_load_image(widget, image, &img) == RET_OK) {
      vgcanvas_paint(vg, FALSE, &img);
    } else {
      vgcanvas_set_fill_color(vg, color);
      vgcanvas_fill(vg);
    }
  } else {
    vgcanvas_set_fill_color(vg, color);
    vgcanvas_fill(vg);
  }

  return RET_OK;
}

static ret_t progress_polygon_draw_fg(widget_t* widget, vgcanvas_t* vg, color_t fg_color,
                                      const char* fg_image, int32_t offset,
                                      const polygon_point_t* end) {
  int32_t i = 0;
  int32_t n = 0;
  bool_t include_end = FALSE;
  polygon_point_t* iter = NULL;
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && vg != NULL && end != NULL, RET_BAD_PARAMS);

  iter = progress_polygon->points.points + offset;
  include_end = iter->value > end->value;
  n = include_end ? offset : offset + 1;

  iter = progress_polygon->points.points;
  vgcanvas_begin_path(vg);
  progress_polygon_move_to(widget, vg, iter->x1, iter->y1);
  for (i = 1; i < n; i++) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x1, iter->y1);
  }

  if (include_end) {
    progress_polygon_line_to(widget, vg, end->x1, end->y1);
    progress_polygon_line_to(widget, vg, end->x2, end->y2);
  }

  for (i = n - 1; i >= 0; i--) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x2, iter->y2);
  }

  vgcanvas_close_path(vg);
  progress_polygon_fill(widget, vg, fg_color, fg_image);

  return RET_OK;
}

static ret_t progress_polygon_draw_bg(widget_t* widget, vgcanvas_t* vg, color_t bg_color,
                                      const char* bg_image, int32_t offset,
                                      const polygon_point_t* start) {
  int32_t i = 0;
  int32_t n = 0;
  polygon_point_t* iter = NULL;
  bool_t include_start = FALSE;
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && vg != NULL && start != NULL, RET_BAD_PARAMS);

  iter = progress_polygon->points.points + offset;
  include_start = iter->value > start->value;

  vgcanvas_begin_path(vg);
  if (include_start) {
    progress_polygon_move_to(widget, vg, start->x1, start->y1);
  } else {
    progress_polygon_move_to(widget, vg, iter->x1, iter->y1);
  }

  n = progress_polygon->points.size;
  for (i = offset; i < n; i++) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x1, iter->y1);
  }

  for (i = n - 1; i >= offset; i--) {
    iter = progress_polygon->points.points + i;
    progress_polygon_line_to(widget, vg, iter->x2, iter->y2);
  }

  if (include_start) {
    progress_polygon_line_to(widget, vg, start->x2, start->y2);
  }

  vgcanvas_close_path(vg);
  progress_polygon_fill(widget, vg, bg_color, bg_image);

  return RET_OK;
}

static ret_t progress_polygon_on_paint_background(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

static ret_t progress_polygon_on_paint_border(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

static ret_t progress_polygon_on_paint_self(widget_t* widget, canvas_t* c) {
  uint32_t offset = 0;
  double value = 0;
  double progress = 0;
  double interpolate = 0;
  style_t* style = widget->astyle;
  polygon_point_t* next = NULL;
  polygon_point_t* prev = NULL;
  polygon_point_t boundary_point = {0, 0, 0, 0};
  color_t transparent = color_init(0x00, 0x00, 0x00, 0x00);
  color_t bg_color = style_get_color(style, STYLE_ID_BG_COLOR, transparent);
  color_t fg_color = style_get_color(style, STYLE_ID_FG_COLOR, transparent);
  color_t border_color = style_get_color(style, STYLE_ID_BORDER_COLOR, transparent);
  const char* bg_image = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  const char* fg_image = style_get_str(style, STYLE_ID_FG_IMAGE, NULL);
  uint32_t line_width = style_get_int(style, STYLE_ID_BORDER_WIDTH, 1);
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  return_value_if_fail(progress_polygon != NULL && vg != NULL, RET_BAD_PARAMS);
  return_value_if_fail(progress_polygon->points.size > 0, RET_BAD_PARAMS);
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);
  return_value_if_fail(progress_polygon->max > progress_polygon->min, RET_BAD_PARAMS);

  value = tk_clamp(progress_polygon->value, progress_polygon->min, progress_polygon->max);
  progress = (value - progress_polygon->min) / (progress_polygon->max - progress_polygon->min);
  offset = polygon_points_find(&progress_polygon->points, progress);

  next = progress_polygon->points.points + offset;
  prev = offset > 0 ? progress_polygon->points.points + offset - 1 : next;
  return_value_if_fail(next != NULL, RET_BAD_PARAMS);

  if (prev != next) {
    interpolate = (progress - prev->value) / (next->value - prev->value);
    boundary_point.value = progress;
    boundary_point.x1 = prev->x1 + (next->x1 - prev->x1) * interpolate;
    boundary_point.y1 = prev->y1 + (next->y1 - prev->y1) * interpolate;
    boundary_point.x2 = prev->x2 + (next->x2 - prev->x2) * interpolate;
    boundary_point.y2 = prev->y2 + (next->y2 - prev->y2) * interpolate;
  } else {
    boundary_point = *next;
  }

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  if (progress > 0 && (fg_color.rgba.a > 0 || fg_image != NULL)) {
    progress_polygon_draw_fg(widget, vg, fg_color, fg_image, offset, &boundary_point);
  }

  if (progress < 1 && (bg_color.rgba.a > 0 || bg_image != NULL)) {
    progress_polygon_draw_bg(widget, vg, bg_color, bg_image, offset, &boundary_point);
  }

  if (border_color.rgba.a > 0) {
    pogress_polygon_draw_border(widget, vg, border_color, line_width);
  }
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t progress_polygon_on_event(widget_t* widget, event_t* e) {
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(widget != NULL && progress_polygon != NULL, RET_BAD_PARAMS);

  (void)progress_polygon;

  return RET_OK;
}

const char* s_progress_polygon_properties[] = {PROGRESS_POLYGON_PROP_VALUE,
                                               PROGRESS_POLYGON_PROP_MIN, PROGRESS_POLYGON_PROP_MAX,
                                               PROGRESS_POLYGON_PROP_POLYGON, NULL};

TK_DECL_VTABLE(progress_polygon) = {.size = sizeof(progress_polygon_t),
                                    .type = WIDGET_TYPE_PROGRESS_POLYGON,
                                    .clone_properties = s_progress_polygon_properties,
                                    .persistent_properties = s_progress_polygon_properties,
                                    .parent = TK_PARENT_VTABLE(widget),
                                    .create = progress_polygon_create,
                                    .on_paint_self = progress_polygon_on_paint_self,
                                    .on_paint_background = progress_polygon_on_paint_background,
                                    .on_paint_border = progress_polygon_on_paint_border,
                                    .set_prop = progress_polygon_set_prop,
                                    .get_prop = progress_polygon_get_prop,
                                    .on_event = progress_polygon_on_event,
                                    .on_destroy = progress_polygon_on_destroy};

widget_t* progress_polygon_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(progress_polygon), x, y, w, h);
  progress_polygon_t* progress_polygon = PROGRESS_POLYGON(widget);
  return_value_if_fail(progress_polygon != NULL, NULL);

  progress_polygon->max = 100;

  return widget;
}

widget_t* progress_polygon_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, progress_polygon), NULL);

  return widget;
}
