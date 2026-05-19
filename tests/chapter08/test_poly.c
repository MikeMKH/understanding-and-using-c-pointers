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

Test(poly, shape_vtable_direct) {
  Shape *shape = getShapeInstance();
  shape->functions->setX(shape, 10);
  shape->functions->setY(shape, 20);
  cr_assert_eq(shape->functions->getX(shape), 10);
  cr_assert_eq(shape->functions->getY(shape), 20);
  char *description = shape->functions->toString(shape);
  cr_assert_str_eq(description, "Shape at (10, 20)\n");
  free(description);
  shape_free(shape);
}

Test(poly, rectangle_polymorphic) {
  Rectangle *rectangle = getRectangleInstance();
  Shape *base = (Shape*)rectangle;
  shape_setX(base, 30);
  shape_setY(base, 40);
  rectangle_setWidth(rectangle, 50);
  rectangle_setHeight(rectangle, 60);
  char *description = base->functions->toString(base);
  cr_assert_str_eq(description, "Rectangle at (30, 40) with width 50 and height 60\n");
  free(description);
  rectangle_free(rectangle);
}

Test(poly, heterogeneous_collection) {
  Shape *shapes[2];
  shapes[0] = getShapeInstance();
  shapes[1] = (Shape*)getRectangleInstance();

  shape_setX(shapes[0], 1);  shape_setY(shapes[0], 2);
  shape_setX(shapes[1], 3);  shape_setY(shapes[1], 4);
  rectangle_setWidth((Rectangle*)shapes[1], 10);
  rectangle_setHeight((Rectangle*)shapes[1], 20);

  for (int i = 0; i < 2; i++) {
    char *description = shapes[i]->functions->toString(shapes[i]);
    cr_assert_not_null(description);
    free(description);
  }

  shape_free(shapes[0]);
  rectangle_free((Rectangle*)shapes[1]);
}