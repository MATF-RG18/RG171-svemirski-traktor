#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "image.h"

/* Konstante za velicinu traktora. */
#define SMALL_WHEEL_WIDTH (0.25)
#define SMALL_WHEEL_R (0.25)
#define BIG_WHEEL_WIDTH (0.3)
#define BIG_WHEEL_R (0.5)

/* Imena fajlova sa teksturama. */
#define SKY "sky.bmp"
#define ROAD "stars.bmp"

/* Konstante za tajmer. */
#define INTERVAL 10
#define ROAD_TIMER_ID 1

/* Identifikatori tekstura. */
static GLuint textures[2];

/* Dimenzije prozora */
static int window_width, window_height;

/* OpenGL inicijalizacija. */
static void initialize(void);

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_special_key_press(int key, int x, int y);
static void on_timer(int timer_id);

/* Deklaracije funkcija koje iscrtavaju sadrzaj igre. */
static void draw_tractor(void);
static void draw_wheel(float wheel_r, float wheel_width);

/* Koordinate traktora. */
static float x_tractor = 0;
static float y_tractor = 0;
static float z_tractor = 0;

/* Koordinate staze. */
static float x_road = 10; // Mora pocinjati malo iza traktora.
static float y_road = -0.5;
static float z_road = 0;
static float road_width = 6;
static float road_length = 500;
/* Oznaka za da li je igra u toku ili ne. */
static int game_active = 1;

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
    glutTimerFunc(INTERVAL, on_timer, ROAD_TIMER_ID);

    /* Obavlja se OpenGL inicijalizacija. */
    initialize();

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void initialize(void)
{
    /* Objekti koji predstavljaju teksture ucitane iz fajla. */
    Image* image;

    /* Pozicija i boja svetla */
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Postavlja se boja pozadine. */
    glClearColor(0, 0, 0, 0);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucuje se osvetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    /* Inicijalizacija objekta koji ce sadrzati teksture. */
    image = image_init(0, 0);

    /* Generisanje identifikatora teksture. */
    glGenTextures(2, textures);

    /* Kreiranje teksture. */
    image_read(image, SKY);

    /* Prva tekstura. */
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

     /* Kreiranje teksture. */
     image_read(image, ROAD);

    /* Druga tekstura. */
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

     /* Iskljucujemo aktivnu teksturu */
     glBindTexture(GL_TEXTURE_2D, 0);

     /* Unistava se objekat za citanje tekstura iz fajla. */
     image_done(image);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    /* Pritiskom ESC izlazi se iz igre. */
    case 27:
        /* Oslobadjaju se korisceni resursi i zavrsava se program. */
        glDeleteTextures(2, textures);
        exit(0);
        break;
    /* Pritiskom na 'g' (go), zapocinje se igra. */
    case 'g':
    case 'G':
        game_active = 1;
        break;
    /* Pritiskom na 'p' (pause), pauzira se igra. */
    case 'p':
    case 'P':
        game_active = 0;
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

static void on_timer(int timer_id) {
    if (!game_active)
        return;

    /* Pomeramo ravan koja predstavlja put pozitivno u x pravcu, cime dobijamo
     * izluziju pomeranja traktora unapred.
     */
    x_road+=0.1;

    glutPostRedisplay();
    glutTimerFunc(INTERVAL, on_timer, ROAD_TIMER_ID);
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
static void draw_wheels(void)
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
static void draw_cabin(void)
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
static void draw_tractor(void)
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

/* Funkcija koja iscrtava pozadinu. Implementirana je kao dve normalne ravni. */
static void draw_skyplane(void)
{
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    /* Gornja ravan. */
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-5, 0, -10);

        glTexCoord2f(3, 0);
        glVertex3f(-5, 6, -10);

        glTexCoord2f(3, 3);
        glVertex3f(-5, 6, 10);

        glTexCoord2f(0, 3);
        glVertex3f(-5, 0, 10);
    glEnd();

    /* Donja ravan. */
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -1, -10);

        glTexCoord2f(3, 0);
        glVertex3f(10, -1, -10);

        glTexCoord2f(3, 3);
        glVertex3f(10, -1, 10);

        glTexCoord2f(0, 3);
        glVertex3f(-10, -1, 10);
    glEnd();

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}

static void draw_road(void)
{
  glBindTexture(GL_TEXTURE_2D, textures[1]);

  glBegin(GL_QUADS);
      glNormal3f(0, 1, 0);

      glTexCoord2f(0, 0);
      glVertex3f(x_road - road_length, y_road, z_road - road_width/2);

      glTexCoord2f(3, 0);
      glVertex3f(x_road, y_road, z_road - road_width/2);

      glTexCoord2f(3, 3);
      glVertex3f(x_road, y_road, z_road + road_width/2);

      glTexCoord2f(0, 3);
      glVertex3f(x_road - road_length, y_road, z_road + road_width/2);
  glEnd();

  /* Iskljucujemo aktivnu teksturu */
  glBindTexture(GL_TEXTURE_2D, 0);
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

    /* Iscrtavanje neba (pozadine). */
    draw_skyplane();

    /* Iscrtavanje staze. */
    if(game_active){

    }
    draw_road();

    /* Iscrtavanje traktora. */
    draw_tractor();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
