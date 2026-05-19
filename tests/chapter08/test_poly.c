#include <criterion/criterion.h>
#include <stdlib.h>
#include "poly.h"

Test(poly, shape_instance) {
  Shape *shape = getShapeInstance();
  shape_setX(shape, 10);
  shape_setY(shape, 20);
  cr_assert_eq(shape_getX(shape), 10);
  cr_assert_eq(shape_getY(shape), 20);
  char *description = shape_toString(shape);
  cr_assert_str_eq(description, "Shape at (10, 20)\n");
  free(description);
  shape_free(shape);
}

Test(poly, rectangle_instance) {
  Rectangle *rectangle = getRectangleInstance();
  shape_setX((Shape*)rectangle, 30);
  shape_setY((Shape*)rectangle, 40);
  rectangle_setWidth(rectangle, 50);
  rectangle_setHeight(rectangle, 60);
  cr_assert_eq(shape_getX((Shape*)rectangle), 30);
  cr_assert_eq(shape_getY((Shape*)rectangle), 40);
  cr_assert_eq(rectangle_getWidth(rectangle), 50);
  cr_assert_eq(rectangle_getHeight(rectangle), 60);
  char *description = rectangle_toString(rectangle);
  cr_assert_str_eq(description, "Rectangle at (30, 40) with width 50 and height 60\n");
  free(description);
  rectangle_free(rectangle);
}
