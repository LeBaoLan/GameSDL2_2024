#include "headers/geometry.h"

void geometry::renderRectangle(const geometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen){
    SDL_Rect fill_rect = {geo_size.left, geo_size.top, geo_size.width, geo_size.height};
    SDL_SetRenderDrawColor(screen, color_data.red, color_data.green, color_data.blue, 255);
    SDL_RenderFillRect(screen, &fill_rect);
}

void geometry::renderOutline(const geometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen){
    SDL_Rect outRect = {geo_size.left, geo_size.top, geo_size.width, geo_size.height};
    SDL_SetRenderDrawColor(screen, color_data.red, color_data.green, color_data.blue, 255);
    SDL_RenderDrawRect(screen, &outRect);
}