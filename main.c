#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "image.h"
#include "funkcije.h"

/* Imena fajlova sa teksturama. */
#define SKY "sky.bmp"
#define ROAD "stars.bmp"

/* Ostale konstante. */
#define INTERVAL 7
#define ROAD_TIMER_ID 1
#define NUM_OF_OBSTACLES 100

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

/* Koordinate traktora. */
static float x_tractor = 3.0;
static float y_tractor = -0.5;
static float z_tractor = 0;

/* Koordinate staze. */
static float x_road = 10; // Mora pocinjati malo iza traktora.
static float y_road = -1;
static float z_road = 0;
static float road_width = 6;
static float road_length = 500;

/* Koordinate prepreka. */
static int obstacles_x[NUM_OF_OBSTACLES];
static int obstacles_y[NUM_OF_OBSTACLES];
static int obstacles_z[NUM_OF_OBSTACLES];

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

    /* Random seed. */
    srand(time(NULL));

    /* Generisanje koordinata za prepreke. */
    generate_obstacles(NUM_OF_OBSTACLES, obstacles_x, obstacles_y, obstacles_z, y_road);

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
            if(game_active && z_tractor < road_width/2) {
              z_tractor += 1;
              glutPostRedisplay();
            }
            break;
        case GLUT_KEY_RIGHT:
            if(game_active && z_tractor > -road_width/2) {
              z_tractor -= 1;
              glutPostRedisplay();
            }
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
    draw_skyplane(textures[0]);

    /* Iscrtavanje staze. */
    draw_road(textures[1], x_road, y_road, z_road, road_width, road_length);

    /* Iscrtavanje prepreka. */
    for(int i = 0; i < NUM_OF_OBSTACLES; i++){
      glPushMatrix();
        glTranslatef(obstacles_x[i] + x_road, obstacles_y[i], obstacles_z[i]);
        glutSolidSphere(0.35, 40, 40);
      glPopMatrix();
      if(has_crashed(x_tractor, y_tractor, z_tractor, obstacles_x[i] + x_road, obstacles_y[i], obstacles_z[i])) {
        printf("KRAJ IGRE");
        game_active = 0;
      }
    }

    /* Ispisivanje rezultata. */

    /* Iscrtavanje traktora. */
    draw_tractor(x_tractor, y_tractor, z_tractor);

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
