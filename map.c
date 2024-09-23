#include "main.h"
#include <SDL2/SDL_render.h>

void drawMap(Game *game)
{
	int i = 0;
	int j = 0;

	int scaled_gridsize = GRIDSIZE / game->minimap_scaling_factor;

	SDL_Rect r;
	r.w = scaled_gridsize * game->map_width;
	r.h = scaled_gridsize * game->map_height;
	r.x = 0;
	r.y = 0;


	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0xff);
	SDL_RenderFillRect(game->renderer, &r);

	r.w = scaled_gridsize;
	r.h = scaled_gridsize;
	for (i = 0; i < game->map_height; i++)
	{
		for (j = 0; j < game->map_width; j++)
		{
			r.x = scaled_gridsize * j; 
			r.y = scaled_gridsize * i; 

			// SDL_SetRenderDrawColor(game->renderer, 0, 0xff, 0xff, 0xff);
			// SDL_RenderDrawRect(game->renderer, &r);

			SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0xff);
			switch (game->map[i][j])
			{
				case 1:
					SDL_SetRenderDrawColor(game->renderer, 0xff, 0, 0, 0xff);
					break;

				case 2:
					SDL_SetRenderDrawColor(game->renderer, 0, 0xff, 0, 0xff);
					break;

				case 3:
					SDL_SetRenderDrawColor(game->renderer, 0, 0, 0xff, 0xff);
					break;

				case 4:
					SDL_SetRenderDrawColor(game->renderer, 0xff, 0xff, 0, 0xff);
					break;

				case 5:
					SDL_SetRenderDrawColor(game->renderer, 0, 0xff, 0xff, 0xff);
					break;
			}
			if (game->map[i][j])
				SDL_RenderFillRect(game->renderer, &r);

		}
	}
}
