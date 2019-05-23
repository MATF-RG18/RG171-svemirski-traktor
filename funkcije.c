#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "image.h"
#include "funkcije.h"

/* Funkcija za iscrtavanje jednog tocka. */
void draw_wheel(float wheel_r, float wheel_width)
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
void draw_wheels(void)
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
void draw_cabin(void)
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
void draw_tractor(float x, float y, float z)
{
    glPushMatrix();
      glTranslatef(x, y, z);
      glColor3f(0, 0, 0);
      glutWireCube(1);
      glColor3f(0.5, 0.5, 0.5);
      glutSolidCube(1);
      draw_cabin();
      draw_wheels();
    glPopMatrix();
}

/* Funkcija koja iscrtava pozadinu. Implementirana je kao dve ravni. */
void draw_skyplane(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    /* Gornja ravan. */
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -100, -100);

        glTexCoord2f(20, 0);
        glVertex3f(-15, 100, -100);

        glTexCoord2f(20, 20);
        glVertex3f(-15, 100, 100);

        glTexCoord2f(0, 20);
        glVertex3f(-10, -100, 100);
    glEnd();

    /* Donja ravan. */
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-150, 4, -100);

        glTexCoord2f(20, 0);
        glVertex3f(10, -2, -100);

        glTexCoord2f(20, 20);
        glVertex3f(10, -2, 100);

        glTexCoord2f(20, 20);
        glVertex3f(-150, 4, 100);
    glEnd();

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}

/* Funkcija koja iscrtava stazu. */
void draw_road(GLuint texture, float x, float y, float z, float width, float length)
{
  glBindTexture(GL_TEXTURE_2D, texture);

  glBegin(GL_QUADS);
      glNormal3f(0, 1, 0);

      glTexCoord2f(0, 0);
      glVertex3f(x - length, y, z - width/2);

      glTexCoord2f(3, 0);
      glVertex3f(x, y, z - width/2);

      glTexCoord2f(3, 3);
      glVertex3f(x, y, z + width/2);

      glTexCoord2f(0, 3);
      glVertex3f(x - length, y, z + width/2);
  glEnd();

  /* Iskljucujemo aktivnu teksturu */
  glBindTexture(GL_TEXTURE_2D, 0);
}

/* Funkcija za detekciju kolizije. */
int has_crashed(float x_tractor, float y_tractor, float z_tractor, float x, float y, float z)
{
  if(z == z_tractor && x >= x_tractor - 0.5 && x <= x_tractor + 0.5) {
    return 1;
  }
  return 0;
}

/* Funkcija koja generise koordinate n prepreka. */
void generate_obstacles(int n, int obstacles_x[],
      int obstacles_y[], int obstacles_z[], float y_road)
{
  // float distance_between_obstacles = (road_length - 10) / (float)n;
  int x_obstacle = -5;
  int y_obstacle = y_road + 1;
  for(int i = 0; i < n; i++) {
    /* Posto imamo 5 "linija" na stazi, trebaju nam koo od -2 do 2. */
    int z_obstacle = (rand() % 5) - 2;
    obstacles_x[i] = x_obstacle*i;
    obstacles_y[i] = y_obstacle;
    obstacles_z[i] = z_obstacle;
  }
}

/* Funkcija za ispis teksta. */
void print(char *tekst)
{

}
