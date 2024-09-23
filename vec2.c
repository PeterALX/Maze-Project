#include "main.h"

void v_draw(Game *game, const vec2 *v, int start_x, int start_y,
		const SDL_Color *color, float scale)
{
		SDL_SetRenderDrawColor(game->renderer,
				color->r, color->g, color->b, color->a);
		SDL_RenderDrawLine(game->renderer,
				start_x, start_y, v->x * scale + start_x, v->y * scale + start_y);
}

void v_add(vec2 *v1, const vec2 *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
}
void v_subtract(vec2 *v1, const vec2 *v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
}
void v_scale(vec2 *v, float scalar)
{
	v->x *= scalar;
	v->y *= scalar;
}
double v_magnitude(const vec2 *v)
{
	return (sqrt(v->x * v->x + v->y * v->y));
}
void v_unit(vec2 *v)
{
	double mag = v_magnitude(v);

	if (mag == 0)
	{
		v->x = 0.0;
		v->y = 0.0;
		return;
	}
	v->x /= mag;
	v->y /= mag;
}
void v_copy(vec2 *to, const vec2 *from)
{
	to->x = from->x;
	to->y = from->y;
}
