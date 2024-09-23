#include "main.h"

void fillCircle(Game* game, int centerX, int centerY, int radius, SDL_Color color) {
		SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(game->renderer, centerX + x, centerY + y);
            }
        }
    }
}
