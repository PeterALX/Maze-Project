#ifndef __MAZE__
#define __MAZE__

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * struct SDL_Instance - initialize an sdl instance(window and renderer)
 * @window: pointer to an SDL_Window
 * @renderer: pointer to an SDL_Renderer
 */
typedef struct Game
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int window_width;
	int window_height;

	int map_width;
	int map_height;
	int map[100][100]; /* max dimensions are 100 by 100 */
	int minimap_scaling_factor;
} Game;


int init_sdl(Game *game);


/* vec stuff starts here */
#include <math.h>

/**
 * struct vec2 - a 2d vector 
 * @x: the x component of the vec
 * @y: the y component of the vec
 */
typedef struct vec2
{
	float x;
	float y;
} vec2;

void v_draw(Game *game, const vec2 *v, int start_x, int start_y, const SDL_Color *color, float scale);
void v_add(vec2 *v1, const vec2 *v2);
void v_subtract(vec2 *v1, const vec2 *v2);
void v_scale(vec2 *v, float scalar);
double v_magnitude(const vec2 *v);
void v_unit(vec2 *v);
void v_copy(vec2 *to, const vec2 *from);

/* map stuff starts here */
#define GRIDSIZE 40
void drawMap(Game *game);

/* player stuff starts here */
typedef struct Player
{
	vec2 pos;
	vec2 lookat;
	int fov;
	int speed;
	int boost;
	vec2 planeRight;
	/* trying no planeLeft */
} Player;

void init_player(Player *p, float pos_x, float pos_y);
void draw_player(Game *game, const Player *player);
void rotate_player(Player *p, float degrees);
void player_see(Game *game, const Player *p);

/* helper functions */
void fillCircle(Game* game, int centerX, int centerY, int radius, SDL_Color color);

#endif /* !__MAZE__ */
