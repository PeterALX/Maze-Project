#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define MAP {\
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},\
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},\
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}\
	}

/**
 * main - initialize an sdl instance(window and renderer)
 * @argc: number of commandline args
 * @argv: array of command line args
 * Return: 0 on success, 1 on fail
 */
// int main(int argc, char *argv[])
int main(void)
{
	Game game = {.map = MAP};
	game.map_width = 24;
	game.map_height = 24;
	game.minimap_scaling_factor = 4;

	const Uint8 *key_state = SDL_GetKeyboardState(NULL);
	SDL_Event e;

	if (init_sdl(&game) != 0)
		return (1);

	// printf("width %d\n", game.window_width);

	Player p1;
	init_player(&p1, 140.0, 140.0);

	float currentTime = SDL_GetTicks();
	float prevTime = 0;
	float fps = 0;
	float cumfps = 0.0;
	int frames = 0;
	float avgfps;
	while (true)
	{
		fps = 1000 / (currentTime - prevTime);
		if (currentTime == prevTime)
			fps = 60.0;
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		// printf("%f\n", fps);

		cumfps += fps;
		frames++;
		if (fps < 5 && frames > 10)
			break;

		SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(game.renderer);

		SDL_PollEvent(&e);
		if (key_state[SDL_SCANCODE_ESCAPE])
			break;
		if (e.type == SDL_QUIT)
			break;
		if (e.type == SDL_MOUSEMOTION)
		{
			float r_delta = e.motion.xrel * 0.01;
			rotate_player(&p1, r_delta);
		}
		int speed = key_state[SDL_SCANCODE_LSHIFT] ? p1.speed + p1.boost : p1.speed;
		int step = speed;
		step = 1;
		if (key_state[SDL_SCANCODE_W])
		{
			float newPosX =  p1.pos.x + p1.lookat.x * step;
			float newPosY =  p1.pos.y + p1.lookat.y * step;
			int gridX = floor(newPosX / GRIDSIZE);
			int gridY = floor(newPosY / GRIDSIZE);
			if (!(game.map[gridY] && game.map[gridY][gridX])) { // collision detection
				p1.pos.x = newPosX;
				p1.pos.y = newPosY;
			}

		}
		if (key_state[SDL_SCANCODE_S])
		{
			float newPosX =  p1.pos.x - p1.lookat.x * step;
			float newPosY =  p1.pos.y - p1.lookat.y * step;
			int gridX = floor(newPosX / GRIDSIZE);
			int gridY = floor(newPosY / GRIDSIZE);
			if (!(game.map[gridY] && game.map[gridY][gridX])) { // collision detection
				p1.pos.x = newPosX;
				p1.pos.y = newPosY;
			}

		}
		if (key_state[SDL_SCANCODE_A])
		{
			float newPosX =  p1.pos.x + p1.lookat.y * step;
			float newPosY =  p1.pos.y - p1.lookat.x * step;
			int gridX = floor(newPosX / GRIDSIZE);
			int gridY = floor(newPosY / GRIDSIZE);
			if (!(game.map[gridY] && game.map[gridY][gridX])) { // collision detection
				p1.pos.x = newPosX;
				p1.pos.y = newPosY;
			}
		}

		if (key_state[SDL_SCANCODE_D])
		{
			float newPosX =  p1.pos.x - p1.lookat.y * step;
			float newPosY =  p1.pos.y + p1.lookat.x * step;
			int gridX = floor(newPosX / GRIDSIZE);
			int gridY = floor(newPosY / GRIDSIZE);
			if (!(game.map[gridY] && game.map[gridY][gridX])) { // collision detection
				p1.pos.x = newPosX;
				p1.pos.y = newPosY;
			}
		}
		player_see(&game, &p1);
		drawMap(&game);
		draw_player(&game, &p1);

		SDL_RenderPresent(game.renderer);
	}
	avgfps = cumfps / frames;
	printf("avg fps: %f\n", avgfps);

	/* sdl cleanup */
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return (0);
}

/**
 * init_sdl - initialize an sdl instance(window and renderer)
 * @instance: a pointer to an SDL_Instance struct
 * Return: 0 on success, 1 on fail
 */
int init_sdl(Game *game)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/* Create a new Window instance */
	game->window = SDL_CreateWindow("SDL2 \\o/", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (game->window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	/* Create a new Renderer instance linked to the Window */
	game->renderer = SDL_CreateRenderer(game->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (game->renderer == NULL)
	{
		SDL_DestroyWindow(game->window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	// SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

	SDL_GetWindowSize(game->window, &game->window_width, &game->window_height);
	return (0);
}
