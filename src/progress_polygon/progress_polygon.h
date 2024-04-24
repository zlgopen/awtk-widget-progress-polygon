/**
 * File:   progress_polygon.h
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


#ifndef TK_PROGRESS_POLYGON_H
#define TK_PROGRESS_POLYGON_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _polygon_point_t {
  double value;
  float x1;
  float y1;
  float x2;
  float y2;
} polygon_point_t;

/*format [(0, 0, 0, 0, 30), (1, 100, 0, 100, 30)]*/

typedef struct _polygon_point_array_t {
  uint32_t size;
  uint32_t capacity;
  polygon_point_t* points;
} polygon_points_t;

/**
 * @class progress_polygon_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 异形进度条。
 * 在xml中使用"progress\_polygon"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <progress_polygon x="c" y="50" w="100" h="40" polygon="(0, 0,1,0,1)(1, 1,0,1,1)"/>
 * 
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <progress_polygon>
 * <style name="default" bg_color="#E0E0E0" fg_color="gold" border_color="green" border_width="2">
 *   <normal />
 * </style>
 * </progress_polygon>
 * ```
 */
typedef struct _progress_polygon_t {
  widget_t widget;


  /**
   * @property {double} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。。
   */
  double value;

  /**
   * @property {double} min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小值。。
   */
  double min;

  /**
   * @property {double} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值。。
   */
  double max;

  /**
   * @property {char*} polygon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 多边形描述(请参考README.md)。
   */
  char* polygon;

  /*private*/
  polygon_points_t points;
} progress_polygon_t;

/**
 * @method progress_polygon_create
 * @annotation ["constructor", "scriptable"]
 * 创建progress_polygon对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} progress_polygon对象。
 */
widget_t* progress_polygon_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method progress_polygon_cast
 * 转换为progress_polygon对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget progress_polygon对象。
 *
 * @return {widget_t*} progress_polygon对象。
 */
widget_t* progress_polygon_cast(widget_t* widget);


/**
 * @method progress_polygon_set_value
 * 设置 值。。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {double} value 值。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_polygon_set_value(widget_t* widget, double value);

/**
 * @method progress_polygon_set_min
 * 设置 最小值。。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {double} min 最小值。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_polygon_set_min(widget_t* widget, double min);

/**
 * @method progress_polygon_set_max
 * 设置 最大值。。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {double} max 最大值。。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_polygon_set_max(widget_t* widget, double max);

/**
 * @method progress_polygon_set_polygon
 * 设置 多边形描述。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {const char*} polygon 多边形描述。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t progress_polygon_set_polygon(widget_t* widget, const char* polygon);

#define PROGRESS_POLYGON_PROP_VALUE "value"
#define PROGRESS_POLYGON_PROP_MIN "min"
#define PROGRESS_POLYGON_PROP_MAX "max"
#define PROGRESS_POLYGON_PROP_POLYGON "polygon"

#define WIDGET_TYPE_PROGRESS_POLYGON "progress_polygon"

#define PROGRESS_POLYGON(widget) ((progress_polygon_t*)(progress_polygon_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(progress_polygon);

/*public for test*/

/**
 * @method polygon_points_init
 * 解析多边形描述。
 * @param {polygon_points_t*} arr 多边形描述。
 * @param {const char*} data 多边形描述。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t polygon_points_init(polygon_points_t* arr, const char* data);

/**
 * @method polygon_points_deinit
 * 释放多边形描述。
 * @param {polygon_points_t*} arr 多边形描述。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t polygon_points_deinit(polygon_points_t* arr);

END_C_DECLS

#endif /*TK_PROGRESS_POLYGON_H*/
