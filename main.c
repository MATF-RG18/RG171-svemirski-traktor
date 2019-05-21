#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* OpenGL inicijalizacija. */
static void initialize(void);

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

/* Deklaracije funkcija koje iscrtavaju sadrzaj igre. */
static void draw_tractor(void);

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
    glClearColor(0, 0, 0, 0);

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

/* Funkcija za iscrtavanje traktora. */
void draw_tractor()
{
    glPushMatrix();
      glTranslatef(3.0, 0.0, 0.0);
      glColor3f(255, 0, 0);
      glutWireCube(1);
      glColor3f(0, 255, 0);
      glutSolidCube(1);
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
    glPushMatrix();
      draw_tractor();
    glPopMatrix();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
