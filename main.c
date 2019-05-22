#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define SMALL_WHEEL_WIDTH (0.25)
#define SMALL_WHEEL_R (0.25)
#define BIG_WHEEL_WIDTH (0.3)
#define BIG_WHEEL_R (0.5)

/* Dimenzije prozora */
static int window_width, window_height;

/* OpenGL inicijalizacija. */
static void initialize(void);

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_special_key_press(int key, int x, int y);

/* Deklaracije funkcija koje iscrtavaju sadrzaj igre. */
static void draw_tractor(void);
static void draw_wheel(float wheel_r, float wheel_width);

/* Koordinate traktora */
static float x_tractor = 0;
static float y_tractor = 0;
static float z_tractor = 0;

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Svemirski traktor");
    glutSetIconTitle("Svemirski traktor");

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutSpecialFunc(on_special_key_press);

    /* Obavlja se OpenGL inicijalizacija. */
    initialize();

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void initialize(void)
{
    /* Pozicija i boja svetla */
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Postavlja se boja pozadine. */
    glClearColor(0, 0, 0.3, 0);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucujemo koriscenje glColor definisanih boja za materijale */
    glEnable(GL_COLOR_MATERIAL);

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    /* Pritiskom ESC izlazi se iz igre. */
    case 27:
        exit(0);
        break;
    }
}

static void on_special_key_press(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
          break;
        case GLUT_KEY_DOWN:
          break;
        case GLUT_KEY_LEFT:
            z_tractor += 1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            z_tractor -= 1;
            glutPostRedisplay();
            break;
    }
}

static void on_reshape(int width, int height)
{
    /* Pamti se velicina prozora. */
    window_width = width;
    window_height = height;

    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) width / height, 1, 25);
}

/* Funkcija za iscrtavanje jednog tocka. */
static void draw_wheel(float wheel_r, float wheel_width)
{
    static GLUquadric * quad;
    quad = gluNewQuadric();

    glColor3f(0.2, 0.2, 0.2);

    glPushMatrix();
      glTranslatef(0, 0, wheel_width/2);
      gluDisk(quad, /*inner=*/0, /*outer=*/ wheel_r, /*slices=*/ 40, /*loops=*/40);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0,0,-wheel_width/2);
      gluDisk(quad, /*inner=*/0, /*outer=*/ wheel_r, /*slices=*/ 40, /*loops=*/40);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,0,-wheel_width/2);
        gluCylinder(quad, wheel_r, wheel_r, wheel_width, 40, 40);
    glPopMatrix();
}

/* Iscrtavanje svih tockova na traktoru. */
static void draw_wheels()
{
  glPushMatrix();
    glTranslatef(0.5, 0, 0.5 + BIG_WHEEL_WIDTH/2);
    draw_wheel(BIG_WHEEL_R, BIG_WHEEL_WIDTH);
    glTranslatef(0, 0, -1.0 - BIG_WHEEL_WIDTH);
    draw_wheel(BIG_WHEEL_R, BIG_WHEEL_WIDTH);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(-0.5, 0, 0.5 + SMALL_WHEEL_WIDTH/2);
    draw_wheel(SMALL_WHEEL_R, SMALL_WHEEL_WIDTH);
    glTranslatef(0, 0, -1.0 - SMALL_WHEEL_WIDTH);
    draw_wheel(SMALL_WHEEL_R, SMALL_WHEEL_WIDTH);
  glPopMatrix();
}

/* Iscrtavanje kabine na traktoru. */
static void draw_cabin()
{
  glPushMatrix();
    glTranslatef(0.25, 0.75, 0);
    glColor3f(0.5, 0.5, 0.5);
    glutSolidCube(0.5);
    glColor3f(0, 0, 0);
    glutWireCube(0.5);
  glPopMatrix();
}

/* Funkcija za iscrtavanje traktora. */
void draw_tractor()
{
    glPushMatrix();
      glTranslatef(x_tractor + 3.0, y_tractor, z_tractor);
      glColor3f(0, 0, 0);
      glutWireCube(1);
      glColor3f(0.5, 0.5, 0.5);
      glutSolidCube(1);
      draw_cabin();
      draw_wheels();
    glPopMatrix();
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Postavljanje perspektive. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 3.0, 0.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    /* Iscrtavanje traktora. */
    draw_tractor();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
