/* Konstante za velicinu traktora. */
#define SMALL_WHEEL_WIDTH (0.25)
#define SMALL_WHEEL_R (0.25)
#define BIG_WHEEL_WIDTH (0.3)
#define BIG_WHEEL_R (0.5)

/* Funkcije za iscrtavanje. */
void draw_wheel(float wheel_r, float wheel_width);
void draw_wheels(void);
void draw_cabin(void);
void draw_tractor(float x, float y, float z);
void draw_skyplane(GLuint texture);
void draw_road(GLuint texture, float x, float y, float z, float width, float length);

/* Ostale funkcije. */
int has_crashed(float x_tractor, float y_tractor, float z_tractor, float x, float y, float z);
void generate_obstacles(int n, int obstacles_x[],
    int obstacles_y[], int obstacles_z[], float y_road);
void print(char *tekst);
