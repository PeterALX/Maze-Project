#include "main.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

void init_player(Player *p, float pos_x, float pos_y)
{
	p->pos.x = pos_x;
	p->pos.y = pos_y;

	p->lookat.x = 1.0; 
	p->lookat.y = 1.0; 
	v_unit(&p->lookat);

	p->fov = 60;
	p->speed = 100;
	p->boost = 200;

	p->planeRight.x = -p->lookat.y;
	p->planeRight.y = p->lookat.x;
	float fov_radians = p->fov * (M_PI / 180);
	float planeLength = tan(fov_radians / 2) * v_magnitude(&p->lookat);
	v_scale(&p->planeRight, planeLength);
}

void draw_player(Game *game, const Player *p)
{
	int scale = 10;
	SDL_Color c1 = {0xff, 0, 0, 0xff};
	SDL_Color c2 = {0, 0xff, 0, 0xff};
	SDL_Color c3 = {0, 0, 0xff, 0xff};

	float map_posx = p->pos.x / game->minimap_scaling_factor;
	float map_posy = p->pos.y / game->minimap_scaling_factor;
	fillCircle(game, map_posx, map_posy, 4, c1);
	v_draw(game, &p->pos, 0, 0, &c1, 1.0 / game->minimap_scaling_factor);
	v_draw(game, &p->lookat, map_posx, map_posy, &c2, scale);

	/*draw projection plane */
	float plane_start_x = map_posx + p->lookat.x * scale; 
	float plane_start_y = map_posy + p->lookat.y * scale; 
	v_draw(game, &p->planeRight, plane_start_x, plane_start_y, &c3, scale);
	v_draw(game, &p->planeRight, plane_start_x, plane_start_y, &c3, -scale);
}

void rotate_player(Player *p, float degrees)
{
	/* rotation matrix */
	float ix = cos(degrees);
	float iy = sin(degrees);
	float jx = -sin(degrees);
	float jy = cos(degrees);

	float new_x = p->lookat.x * ix + p->lookat.y * jx;
	float new_y = p->lookat.x * iy + p->lookat.y * jy;

	p->lookat.x = new_x;
	p->lookat.y = new_y;

	p->planeRight.x = -p->lookat.y;
	p->planeRight.y = p->lookat.x;
	float fov_radians = p->fov * (M_PI / 180);
	float planeLength = tan(fov_radians / 2) * v_magnitude(&p->lookat);
	v_scale(&p->planeRight, planeLength);
}

void player_see(Game *game, const Player *p)
{
	vec2 ray;
	vec2 next_column = {INFINITY, INFINITY};
	vec2 next_row = {INFINITY, INFINITY};
	vec2 row_delta = {INFINITY, INFINITY};
	vec2 column_delta = {INFINITY, INFINITY};
	vec2 current;
	for (int i = 0; i <= game->window_width; i+=1)
	{

		float camera_x = ((float)i / game->window_width) * 2 - 1;
		v_copy(&ray, &p->planeRight);
		v_scale(&ray, camera_x);
		v_add(&ray, &p->lookat);
		float r_plane = v_magnitude(&ray); /* ray's distance to plane */
		v_unit(&ray);

		if (ray.x != 0)
		{
			float initialColX = ray.x > 0 ? GRIDSIZE - (int)p->pos.x % GRIDSIZE : -((int)p->pos.x % GRIDSIZE);
			float initialColY = (ray.y/ray.x) * initialColX;
			next_column.x = initialColX;
			next_column.y = initialColY;

			float colX = ray.x > 0 ? GRIDSIZE : -GRIDSIZE;
			float colY = (ray.y/ray.x) * colX;
			column_delta.x = colX;
			column_delta.y = colY;
		}
		if (ray.y != 0)
		{
			float initialRowY = ray.y > 0 ? GRIDSIZE - (int)p->pos.y % GRIDSIZE : -((int)p->pos.y % GRIDSIZE);
			float initialRowX = (ray.x/ray.y) * initialRowY;
			next_row.x = initialRowX;
			next_row.y = initialRowY;

			float rowY = ray.y > 0 ? GRIDSIZE : -GRIDSIZE;
			float rowX = (ray.x/ray.y) * rowY;
			row_delta.x = rowX;
			row_delta.y = rowY;
		}

		bool collision = false;
		int col;
		int row;

		while (!collision)
		{
			if (v_magnitude(&next_column) < v_magnitude(&next_row))
			{
				current.x = next_column.x;
				current.y = next_column.y;
				/* v_add(&next_column, &column_delta); */
				next_column.x += column_delta.x;
				next_column.y += column_delta.y;
			}
			else if (v_magnitude(&next_row) < v_magnitude(&next_column))
			{
				current.x = next_row.x;
				current.y = next_row.y;
				/* v_add(&next_row, &row_delta); */
				next_row.x += row_delta.x;
				next_row.y += row_delta.y;
			}
			else
			{
				current.x = next_column.x;
				current.y = next_column.y;

				/* v_add(&next_row, &row_delta); */
				/* v_add(&next_column, &column_delta); */
				next_column.x += column_delta.x;
				next_column.y += column_delta.y;
				next_row.x += row_delta.x;
				next_row.y += row_delta.y;
			}

			float wallX = p->pos.x + current.x;
			float wallY = p->pos.y + current.y;

			col = floor(wallX / GRIDSIZE);
			row = floor(wallY / GRIDSIZE);
			if (ray.x < 0 && (int)wallX % GRIDSIZE == 0)
			{ /*compress to ternary */
				col -= 1;
			}
			if (ray.y < 0 && (int)wallY % GRIDSIZE == 0)
			{
				row -= 1;
			}
			if (row < 0 || row >= game->map_height)
			{ /*compress to one if */
				break;
			}
			if (col < 0 || col >= game->map_width)
			{
				break;
			}
			if (game->map[row][col])
			{
				collision = true;
				break;
			}
		}
		if (collision)
		{
			/*draw map*/

			/*draw scene*/
			switch (game->map[row][col])
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

			float dWall = v_magnitude(&current);
			float perpWall = (dWall / r_plane)
				* v_magnitude(&p->lookat); /* correcting fisheye effect */
			float actualWallHeight = 64; /* I don't completely get this mazematik */
			float dPlane = 355; /* distance from player to projection plane */
			float projectedHeight = floor((actualWallHeight / perpWall) * dPlane);
			int playerHeight = game->window_height / 2;

			SDL_RenderDrawLine(game->renderer,
					i, playerHeight + projectedHeight / 2,
					i, playerHeight - projectedHeight / 2);
		}
		/* v_draw(game, &current, p->pos.x, p->pos.y, &c, 1); */
	}
}
