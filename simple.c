/* X compile line: cc -o simple simple.c -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm */
#include <GL/glut.h>
#include <math.h>

struct COLOR_
{
  float r_;
  float g_;
  float b_;
};

struct QUADE_POS_
{
  int x_pos[4];
  int y_pos[4];
};

struct CICLE_
{
  int sides_; // Número de lados do círculo
  float r_; // Raio do círculo
  int x_; // Coordenada x do centro do círculo
  int y_; // Coordenada y do centro do círculo
};

typedef struct COLOR_ COLOR;
typedef struct QUADE_POS_ QUADE_POS;
typedef struct CICLE_ CICLE;

COLOR colorQuade1;
QUADE_POS quadePos1;
CICLE cicle1;
int key_C = 0;

void
init()
{
  colorQuade1.r_ = 0.0;
  colorQuade1.g_ = 0.1;
  colorQuade1.b_ = 0.0;

  quadePos1.x_pos[0] = 100;
  quadePos1.x_pos[1] = 100;
  quadePos1.x_pos[2] = 0;
  quadePos1.x_pos[3] = 0;

  quadePos1.y_pos[0] = 0;
  quadePos1.y_pos[1] = 100;
  quadePos1.y_pos[2] = 100;
  quadePos1.y_pos[3] = 0;

  cicle1.sides_ = 100;
  cicle1.r_ = 50;
  cicle1.x_ = 50;
  cicle1.y_ = 50;
}

void 
reshape(int w, int h)
{
  glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
  glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
  glLoadIdentity();             /* Reset project matrix. */
  glOrtho(0, w, 0, h, -1, 1);   /* Map abstract coords directly to window coords. */
  glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
  glTranslatef(0, -h, 0);       /* Shift origin up to upper-left corner. */
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  if(!key_C)
  {
    glBegin(GL_QUADS);
      glColor3f(colorQuade1.r_, colorQuade1.g_, colorQuade1.b_);
      glVertex2i(quadePos1.x_pos[0], quadePos1.y_pos[0]);
      glColor3f(colorQuade1.r_, colorQuade1.g_, colorQuade1.b_);
      glVertex2i(quadePos1.x_pos[1], quadePos1.y_pos[1]);
      glColor3f(colorQuade1.r_, colorQuade1.g_, colorQuade1.b_);
      glVertex2i(quadePos1.x_pos[2], quadePos1.y_pos[2]);
      glColor3f(colorQuade1.r_, colorQuade1.g_, colorQuade1.b_);
      glVertex2i(quadePos1.x_pos[3], quadePos1.y_pos[3]);
  }
  else
  {
    glColor3f(colorQuade1.r_, colorQuade1.g_, colorQuade1.b_);
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(cicle1.x_, cicle1.y_); // Centro do círculo
      for (int i = 0; i <= cicle1.sides_; ++i) {
        float angle = 2.0f * M_PI * i / cicle1.sides_;
        float x = cicle1.x_ + cicle1.r_ * cos(angle);
        float y = cicle1.y_ + cicle1.r_ * sin(angle);
        glVertex2f(x, y);
      }
  }
  glEnd();
  glFlush();  /* Single buffered, so needs a flush. */
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
    colorQuade1.r_ += 0.1;
    glutPostRedisplay();    
    break;
  case 'R':
    colorQuade1.r_ -= 0.1;
    glutPostRedisplay();    
    break;
  case 'g':
    colorQuade1.g_ += 0.1;
    glutPostRedisplay();
    break;
  case 'G':
    colorQuade1.g_ -= 0.1;
    glutPostRedisplay();
    break;
  case 'b':
    colorQuade1.b_ += 0.1;
    glutPostRedisplay();
    break;
  case 'B':
    colorQuade1.b_ -= 0.1;
    glutPostRedisplay();
    break;
  case 'w':
    for(int i=0; i<4; i++)
    {
      quadePos1.y_pos[i]-=10;
    }
    cicle1.y_-=10;
    glutPostRedisplay();
    break;
  case 'a':
    for(int i=0; i<4; i++)
    {
      quadePos1.x_pos[i]-=10;
    }
    cicle1.x_-=10;
    glutPostRedisplay();
    break;
  case 's':
    for(int i=0; i<4; i++)
    {
      quadePos1.y_pos[i]+=10;
    }
    cicle1.y_+=10;
    glutPostRedisplay();
    break;
  case 'd':
    for(int i=0; i<4; i++)
    {
      quadePos1.x_pos[i]+=10;
    }
    cicle1.x_+=10;
    glutPostRedisplay();
    break;
  case 'c':
    key_C = 1;
    glutPostRedisplay();
    break;
  case 'C':
    key_C = 0;
    glutPostRedisplay();
    break;
  }
}

void 
keyReleased(unsigned char key, int x, int y) 
{
  switch (key) 
  {
  case 'z': 
    init();
    glutPostRedisplay();
    break;
  }
}

int
main(int argc, char **argv)
{ 

  init();
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("single triangle");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyReleased);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
