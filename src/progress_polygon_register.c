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
#include "progress_polygon_register.h"
#include "base/widget_factory.h"
#include "progress_polygon/progress_polygon.h"

ret_t progress_polygon_register(void) {
  return widget_factory_register(widget_factory(), WIDGET_TYPE_PROGRESS_POLYGON, progress_polygon_create);
}

const char* progress_polygon_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}
