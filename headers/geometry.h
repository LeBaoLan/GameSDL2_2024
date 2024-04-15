#pragma once

#include "baseObject.h"
#include "commonFunc.h"

class geometry{
    public:
        static void renderRectangle(const geometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen);
        static void renderOutline(const geometricFormat& geo_size, const colorData& color_data, SDL_Renderer* screen);
};