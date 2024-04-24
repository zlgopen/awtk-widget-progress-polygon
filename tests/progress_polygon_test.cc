#include "progress_polygon/progress_polygon.h"
#include "gtest/gtest.h"

TEST(progress_polygon, parse0) {
  polygon_points_t points;
  const char* data = "()";
  ret_t ret = polygon_points_init(&points, data);
  EXPECT_EQ(ret, RET_OK);
  EXPECT_EQ(points.size, 1);
  EXPECT_EQ(points.capacity, 1);
  EXPECT_EQ(points.points[0].value, 0);
  EXPECT_EQ(points.points[0].x1, 0);
  EXPECT_EQ(points.points[0].y1, 0);
  EXPECT_EQ(points.points[0].x2, 0);
  EXPECT_EQ(points.points[0].y2, 0);

  polygon_points_deinit(&points);
}

TEST(progress_polygon, parse1) {
  polygon_points_t points;
  const char* data = "(0,1,2,3,4)";
  ret_t ret = polygon_points_init(&points, data);
  EXPECT_EQ(ret, RET_OK);
  EXPECT_EQ(points.size, 1);
  EXPECT_EQ(points.capacity, 1);
  EXPECT_EQ(points.points[0].value, 0);
  EXPECT_EQ(points.points[0].x1, 1);
  EXPECT_EQ(points.points[0].y1, 2);
  EXPECT_EQ(points.points[0].x2, 3);
  EXPECT_EQ(points.points[0].y2, 4);

  polygon_points_deinit(&points);
}

TEST(progress_polygon, parse2) {
  polygon_points_t points;
  const char* data = "(0 , 1 , 2 , 3 , 4)";
  ret_t ret = polygon_points_init(&points, data);
  EXPECT_EQ(ret, RET_OK);
  EXPECT_EQ(points.size, 1);
  EXPECT_EQ(points.capacity, 1);
  EXPECT_EQ(points.points[0].value, 0);
  EXPECT_EQ(points.points[0].x1, 1);
  EXPECT_EQ(points.points[0].y1, 2);
  EXPECT_EQ(points.points[0].x2, 3);
  EXPECT_EQ(points.points[0].y2, 4);

  polygon_points_deinit(&points);
}

TEST(progress_polygon, parse3) {
  polygon_points_t points;
  const char* data = "(0 , 1 , 2 , 3 , 4), (0.5 , 1.1 , 2.2 , 3.3 , 4.4)";
  ret_t ret = polygon_points_init(&points, data);
  EXPECT_EQ(ret, RET_OK);
  EXPECT_EQ(points.size, 2);
  EXPECT_EQ(points.capacity, 2);
  EXPECT_EQ(points.points[0].value, 0);
  EXPECT_EQ(points.points[0].x1, 1);
  EXPECT_EQ(points.points[0].y1, 2);
  EXPECT_EQ(points.points[0].x2, 3);
  EXPECT_EQ(points.points[0].y2, 4);

   EXPECT_EQ(points.capacity, 2);
  EXPECT_EQ(tk_fequal(points.points[1].value, 0.5), TRUE);
  EXPECT_EQ(tk_fequal(points.points[1].x1, 1.1), TRUE);
  EXPECT_EQ(tk_fequal(points.points[1].y1, 2.2), TRUE);
  EXPECT_EQ(tk_fequal(points.points[1].x2, 3.3), TRUE);
  EXPECT_EQ(tk_fequal(points.points[1].y2, 4.4), TRUE);

  polygon_points_deinit(&points);
}

TEST(progress_polygon, basic) {
  widget_t* w = progress_polygon_create(NULL, 10, 20, 30, 40);

  widget_destroy(w);
}
