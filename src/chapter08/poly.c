#include "poly.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _shape {
  vFunctions *functions;
  int x;
  int y;
} Shape;

typedef struct _functions {
  fptrSet setX;
  fptrSet setY;
  fptrGet getX;
  fptrGet getY;
  fptrToString toString;
} vFunctions;

char* shapeToString(void *shape) {
  Shape *s = (Shape*)shape;
  char* buffer = malloc(100);
  snprintf(buffer, 100, "Shape at (%d, %d)\n", s->x, s->y);
  return buffer;
}
void shapeSetX(void *shape, int x) { ((Shape*)shape)->x = x; }
void shapeSetY(void *shape, int y) { ((Shape*)shape)->y = y; }
int shapeGetX(void *shape) { return ((Shape*)shape)->x; }
int shapeGetY(void *shape) { return ((Shape*)shape)->y; }

Shape* getShapeInstance(void) {
  Shape *shape = malloc(sizeof(Shape));
  shape->functions = malloc(sizeof(vFunctions));
  shape->functions->setX = shapeSetX;
  shape->functions->setY = shapeSetY;
  shape->functions->getX = shapeGetX;
  shape->functions->getY = shapeGetY;
  shape->functions->toString = shapeToString;
  return shape;
}

void  shape_setX(Shape *s, int x) { s->functions->setX(s, x); }
void  shape_setY(Shape *s, int y) { s->functions->setY(s, y); }
int   shape_getX(Shape *s) { return s->functions->getX(s); }
int   shape_getY(Shape *s) { return s->functions->getY(s); }
char* shape_toString(Shape *s) { return s->functions->toString(s); }
void  shape_free(Shape *s) { free(s->functions); free(s); }

typedef struct _rectangle {
  Shape base;
  int width;
  int height;
  fptrGet getWidth;
  fptrGet getHeight;
  fptrSet setWidth;
  fptrSet setHeight;
} Rectangle;

char* rectangleToString(void *rectangle) {
  Rectangle *r = (Rectangle*)rectangle;
  char* buffer = malloc(100);
  snprintf(buffer, 100, "Rectangle at (%d, %d) with width %d and height %d\n", r->base.x, r->base.y, r->width, r->height);
  return buffer;
}
void rectangleSetWidth(void *rectangle, int width) { ((Rectangle*)rectangle)->width = width; }
void rectangleSetHeight(void *rectangle, int height) { ((Rectangle*)rectangle)->height = height; }
int rectangleGetWidth(void *rectangle) { return ((Rectangle*)rectangle)->width; }
int rectangleGetHeight(void *rectangle) { return ((Rectangle*)rectangle)->height; }

Rectangle* getRectangleInstance(void) {
  Rectangle *rectangle = malloc(sizeof(Rectangle));
  rectangle->base.functions = malloc(sizeof(vFunctions));
  rectangle->base.functions->setX = shapeSetX;
  rectangle->base.functions->setY = shapeSetY;
  rectangle->base.functions->getX = shapeGetX;
  rectangle->base.functions->getY = shapeGetY;
  rectangle->base.functions->toString = rectangleToString;
  rectangle->width = 0;
  rectangle->height = 0;
  rectangle->setWidth = rectangleSetWidth;
  rectangle->setHeight = rectangleSetHeight;
  rectangle->getWidth = rectangleGetWidth;
  rectangle->getHeight = rectangleGetHeight;
  return rectangle;
}

void  rectangle_setWidth(Rectangle *r, int width) { r->setWidth(r, width); }
void  rectangle_setHeight(Rectangle *r, int height) { r->setHeight(r, height); }
int   rectangle_getWidth(Rectangle *r) { return r->getWidth(r); }
int   rectangle_getHeight(Rectangle *r) { return r->getHeight(r); }
char* rectangle_toString(Rectangle *r) { return r->base.functions->toString(r); }
void rectangle_free(Rectangle *r) { free(r->base.functions); free(r); }