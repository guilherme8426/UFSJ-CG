/* X compile line: cc -o casa casa.c -lglut -lGLU -lGL -lXmu -lXext -lX12 -lm */
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define MAX_DIMENSIONS 4
#define STATIC_OBJECT_MAX 20

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

OBJECT staticObject[STATIC_OBJECT_MAX];
OBJECT dinamicObject[5][3];
int staticObjectLen = 0;
int dinamicObjectLen = 0;
int frequence;
int r = 0;

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
initPolygon(OBJECT *object, COLOR color, int dimensions, int x_[], int y_[], void (*draw)(OBJECT self), int staticOrDinamic)
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

  if (staticOrDinamic)
  {
    staticObjectLen++;
  }
  else
  {
    dinamicObjectLen++;
  }

}

void
initCircle(OBJECT *object, COLOR color, int r_, int sides, int x_, int y_, void (*draw)(OBJECT self), int staticOrDinamic)
{
  object->color = color;

  object->dimensions.sides_ = sides;
  object->dimensions.dimensions = 1;
  object->dimensions.x_[0] = x_; 
  object->dimensions.y_[0] = y_; 
  object->dimensions.r_ = r_;

  object->draw = (*draw);

  if (staticOrDinamic)
  {
    staticObjectLen++;
  }
  else
  {
    dinamicObjectLen++;
  }
   
}

void
initCloud(OBJECT *object, int x_[], int y_[], void (*draw)(OBJECT self)){
  COLOR color = initColor(8.0f, 8.0f, 8.0f);

  for (int i = 0; i < 3; i++)
  {
    object[i].color = color;

    object[i].dimensions.sides_ = 100;
    object[i].dimensions.dimensions = 1;
    object[i].dimensions.x_[0] = x_[i]; 
    object[i].dimensions.y_[0] = y_[i]; 
    object[i].dimensions.r_ = 15;

    object[i].draw = (*draw);
  }
  
  dinamicObjectLen++;
}

void
init()
{ 
  //chao
  int x1[] = {800, 800, 0, 0};
  int y1[] = {600, 560, 560, 600};
  initPolygon(&staticObject[0], initColor(0.209804, 0.59804, 0.084314), 4, x1, y1, drawPolygon, 1);

  //quadrado parede
  int x2[] = {200, 200, 50, 50};
  int y2[] = {560, 410, 410, 560};
  initPolygon(&staticObject[1], initColor(0.0f, 0.7f, 0.0f), 4, x2, y2, drawPolygon, 1);

  ////retangulo parede
  int x3[] = {450, 450, 200, 200};
  int y3[] = {560, 410, 410, 560};
  initPolygon(&staticObject[2], initColor(0.0f, 1.0f, 0.0f), 4, x3, y3, drawPolygon, 1);

  //triangulo telhado
  int x4[] = {200, 125, 50};
  int y4[] = {410, 340, 410};
  initPolygon(&staticObject[3], initColor(0.7f, 0.0f, 0.0f), 3, x4, y4, drawPolygon, 1);

  //quadrilatero telhado
  int x5[] = {450, 375, 125, 200};
  int y5[] = {410, 340, 340, 410};
  initPolygon(&staticObject[4], initColor(1.0f, 0.0f, 0.0f), 4, x5, y5, drawPolygon, 1);

  //porta
  int x6[] = {135, 135, 105, 105};
  int y6[] = {560, 500, 500, 560};
  initPolygon(&staticObject[5], initColor(0.6f, 0.2f, 0.0f), 4, x6, y6, drawPolygon, 1);

  //maçaneta
  initCircle(&staticObject[6], initColor(0.0f, 0.0f, 0.0f), 3, 100, 130, 535, drawCircle, 1);

  //janela circulo
  initCircle(&staticObject[7], initColor(0.658824, 0.658824, 0.658824), 22, 100, 125, 383, drawCircle, 1);

  //janela 1 fora
  int x7[] = {300, 300, 220, 220};
  int y7[] = {510, 460, 460, 510};
  initPolygon(&staticObject[8], initColor(0.184314, 0.309804, 0.184314), 4, x7, y7, drawPolygon, 1);

  //janela 1 dentro
  int x8[] = {280, 280, 240, 240};
  int y8[] = {510, 460, 460, 510};
  initPolygon(&staticObject[9], initColor(0.658824, 0.658824, 0.658824), 4, x8, y8, drawPolygon, 1);

  //janela 2 fora
  int x9[] = {430, 430, 350, 350};
  int y9[] = {510, 460, 460, 510};
  initPolygon(&staticObject[10], initColor(0.184314, 0.309804, 0.184314), 4, x9, y9, drawPolygon, 1);

  //janela 2 dentro
  int x10[] = {410, 410, 370, 370};
  int y10[] = {510, 460, 460, 510};
  initPolygon(&staticObject[11], initColor(0.658824, 0.658824, 0.658824), 4, x10, y10, drawPolygon, 1);

  //chamine pequeno
  int x11[] = {270, 270, 230, 230};
  int y11[] = {340, 310, 310, 340};
  initPolygon(&staticObject[12], initColor(1.0f, 0.25f, 0.0f), 4, x11, y11, drawPolygon, 1);

  //chamine grande
  int x12[] = {280, 280, 220, 220};
  int y12[] = {310, 300, 300, 310};
  initPolygon(&staticObject[13], initColor(1.0f, 0.25f, 0.0f), 4, x12, y12, drawPolygon, 1);

  //sol
  initCircle(&staticObject[14], initColor(1.0f, 1.0f, 0.0f), 50, 100, 700, 100, drawCircle, 1);

  int x13[] = {260,  x13[0]-3, x13[0]-20};
  int y13[] = {280, y13[0]-12, y13[0]-2};
  initCloud(dinamicObject[0], x13, y13, drawCircle);

  int x14[] = {260,  x14[0]-3, x14[0]-20};
  int y14[] = {210, y14[0]-12, y14[0]-2};
  initCloud(dinamicObject[1], x14, y14, drawCircle);

  int x15[] = {260,  x15[0]-3, x15[0]-20};
  int y15[] = {140, y15[0]-12, y15[0]-2};
  initCloud(dinamicObject[2], x15, y15, drawCircle);

  int x16[] = {260,  x16[0]-3, x16[0]-20};
  int y16[] = {70, y16[0]-12, y16[0]-2};
  initCloud(dinamicObject[3], x16, y16, drawCircle);

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

  for(int i=0; i<staticObjectLen; i++)
  {
    staticObject[i].draw(staticObject[i]);  
  }
  
  for(int i=0; i<dinamicObjectLen; i++)
  {
    dinamicObject[i][0].draw(dinamicObject[i][0]);  
    dinamicObject[i][1].draw(dinamicObject[i][1]);  
    dinamicObject[i][2].draw(dinamicObject[i][2]);  
  }

  glFlush();  
}

void 
keyPressed(unsigned char key, int x, int y) 
{
  switch (key) 
  {
  case 27: // Tecla Esc (27) - fecha a janela
    exit(0);
    break;
  case 'r':
    r=(r+1)%2;
    if(!r) init();
    break;
  }
}

void 
timer(int value) 
{
  
  for(int i=0; i<dinamicObjectLen; i++)
  {
    int velocidade = 1;
    if(dinamicObject[i][0].dimensions.y_[0] < 0)
    {
      dinamicObject[i][0].dimensions.y_[0] = 280;
      dinamicObject[i][1].dimensions.y_[0] = 280 - 12;
      dinamicObject[i][2].dimensions.y_[0] = 280 -  2;
    }
    else
    {
      dinamicObject[i][0].dimensions.y_[0] -= velocidade;
      dinamicObject[i][1].dimensions.y_[0] -= velocidade;
      dinamicObject[i][2].dimensions.y_[0] -= velocidade;
    }
  }

  if(r)
  {
    for(int i=0; i<staticObjectLen; i++)
    {
      staticObject[i].color.r_ = (rand() % 10)/10.0;
      staticObject[i].color.g_ = (rand() % 10)/10.0;
      staticObject[i].color.b_ = (rand() % 10)/10.0;
    }
    
    for(int i=0; i<dinamicObjectLen; i++)
    {
      dinamicObject[i][0].color.r_ = (rand() % 10)/10.0;
      dinamicObject[i][0].color.g_ = (rand() % 10)/10.0;
      dinamicObject[i][0].color.b_ = (rand() % 10)/10.0;
      dinamicObject[i][1].color.r_ = (rand() % 10)/10.0;
      dinamicObject[i][1].color.g_ = (rand() % 10)/10.0;
      dinamicObject[i][1].color.b_ = (rand() % 10)/10.0;
      dinamicObject[i][2].color.r_ = (rand() % 10)/10.0;
      dinamicObject[i][2].color.g_ = (rand() % 10)/10.0;
      dinamicObject[i][2].color.b_ = (rand() % 10)/10.0;
    }
  } 

  glutPostRedisplay();
  glutTimerFunc(frequence, timer, 0);
}

int
main(int argc, char **argv)
{ 
  frequence = 60;

  init();
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("single triangle");

  glClearColor(0.196078f, 0.6f, 0.8f, 1.0f); // sky blue
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyPressed);
  glutTimerFunc(frequence, timer, 0);
  
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
