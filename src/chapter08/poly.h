typedef void (*fptrSet)(void*, int);
typedef int (*fptrGet)(void*);
typedef char* (*fptrToString)(void*);

typedef struct _functions {
  fptrSet setX;
  fptrSet setY;
  fptrGet getX;
  fptrGet getY;
  fptrToString toString;
} vFunctions;

typedef struct _shape {
  vFunctions *functions;
  int x;
  int y;
} Shape;

typedef struct _rectangle {
  Shape base;
  int width;
  int height;
  fptrGet getWidth;
  fptrGet getHeight;
  fptrSet setWidth;
  fptrSet setHeight;
} Rectangle;

Shape* getShapeInstance(void);

void  shape_setX(Shape *s, int x);
void  shape_setY(Shape *s, int y);
int   shape_getX(Shape *s);
int   shape_getY(Shape *s);
char* shape_toString(Shape *s);
void  shape_free(Shape *s);

typedef struct _rectangle Rectangle;
Rectangle* getRectangleInstance(void);

void  rectangle_setWidth(Rectangle *r, int width);
void  rectangle_setHeight(Rectangle *r, int height);
int   rectangle_getWidth(Rectangle *r);
int   rectangle_getHeight(Rectangle *r);
char* rectangle_toString(Rectangle *r);
void  rectangle_free(Rectangle *r);