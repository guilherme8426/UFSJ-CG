/* X compile line: cc -o casa casa.c -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm */
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define MAX_DIMENSIONS 4
#define STATIC_OBJECT_LEN 6

typedef struct COLOR_ COLOR;
typedef struct DIMENSIONS_ DIMENSIONS;
typedef struct OBJECT_ OBJECT;

struct COLOR_
{
  float r_;
  float g_; 
  float b_;
};

struct DIMENSIONS_{
  int dimensions;
  int x_[MAX_DIMENSIONS];
  int y_[MAX_DIMENSIONS];
  int r_;
  int sides_;
};

struct OBJECT_
{
  COLOR color;
  DIMENSIONS dimensions;
  void (*draw)(OBJECT self);
};

OBJECT staticObject[STATIC_OBJECT_LEN];

void
drawPolygon(OBJECT self)
{
  switch (self.dimensions.dimensions)
  {
  case 3:
    glBegin(GL_TRIANGLES);
    break;

  case 4:
    glBegin(GL_QUADS);
    break;

  default:
    glBegin(GL_POLYGON);
    break;
  }

  for(int i=0; i<self.dimensions.dimensions; i++){
    glColor3f(self.color.r_, self.color.g_, self.color.b_);
    glVertex2i(self.dimensions.x_[i], self.dimensions.y_[i]);
  }

  glEnd();
}

void
drawCircle(OBJECT self)
{ 
  glColor3f(self.color.r_, self.color.g_, self.color.b_);
  glBegin(GL_TRIANGLE_FAN);
    glVertex2f(self.dimensions.x_[0], self.dimensions.y_[0]); 
    for (int i = 0; i <= self.dimensions.sides_; ++i) {
      float angle = 2.0f * M_PI * i / self.dimensions.sides_;
      float x = self.dimensions.x_[0] + self.dimensions.r_ * cos(angle);
      float y = self.dimensions.y_[0] + self.dimensions.r_ * sin(angle);
      glVertex2f(x, y);
    }
  glEnd();
}

COLOR
initColor(float r, float g, float b)
{
  COLOR color;
  color.r_ = r; 
  color.g_ = g; 
  color.b_ = b;
  return color;
}

void
initPolygon(OBJECT *object, COLOR color, int dimensions, int x_[], int y_[], void (*draw)(OBJECT self))
{
  object->color = color;

  object->dimensions.dimensions = dimensions;
  for(int i=0; i<dimensions; i++)
  {
    object->dimensions.x_[i] = x_[i]; 
    object->dimensions.y_[i] = y_[i]; 
  }
  object->dimensions.r_=0;
  object->dimensions.sides_=0;

  object->draw = (*draw);
}

void
initCircle(OBJECT *object, COLOR color, int r_, int sides, int x_, int y_, void (*draw)(OBJECT self))
{
  object->color = color;

  object->dimensions.sides_ = sides;
  object->dimensions.dimensions = 1;
  object->dimensions.x_[0] = x_; 
  object->dimensions.y_[0] = y_; 
  object->dimensions.r_ = r_;

  object->draw = (*draw);
}

void
init()
{ 
  int x1[] = {200, 200, 50, 50};
  int y1[] = {600, 450, 450, 600};
  initPolygon(&staticObject[0], initColor(0.0f, 0.7f, 0.0f), 4, x1, y1, drawPolygon);

  int x2[] = {450, 450, 200, 200};
  int y2[] = {600, 450, 450, 600};
  initPolygon(&staticObject[1], initColor(0.0f, 1.0f, 0.0f), 4, x2, y2, drawPolygon);

  int x3[] = {200, 125, 50};
  int y3[] = {450, 380, 450};
  initPolygon(&staticObject[2], initColor(0.7f, 0.0f, 0.0f), 3, x3, y3, drawPolygon);

  int x4[] = {450, 375, 125, 200};
  int y4[] = {450, 380, 380, 450};
  initPolygon(&staticObject[3], initColor(1.0f, 0.0f, 0.0f), 4, x4, y4, drawPolygon);

  int x5[] = {135, 135, 115, 115};
  int y5[] = {600, 550, 550, 600};
  initPolygon(&staticObject[4], initColor(0.6f, 0.2f, 0.0f), 4, x5, y5, drawPolygon);

  initCircle(&staticObject[5], initColor(0.0f, 0.0f, 1.0f), 50, 100, 100, 100, drawCircle);

}

void 
reshape(int w, int h)
{
  glViewport(0, 0, w, h);      
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();            
  glOrtho(0, w, 0, h, -1, 1);  
  glScalef(1, -1, 1);          
  glTranslatef(0, -h, 0);      
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  for(int i=0; i<STATIC_OBJECT_LEN; i++)
  {
    staticObject[i].draw(staticObject[i]);  
  }

  glFlush();  
}

int
main(int argc, char **argv)
{ 
  init();
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("single triangle");

  glClearColor(0.196078f, 0.6f, 0.8f, 1.0f); // sky blue
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
