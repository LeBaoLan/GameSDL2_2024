// #ifndef COMMON_FUNC_H
// #define COMMON_FUNC_H
#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Music* g_background_music[4];
static Mix_Chunk* g_sound_effects[8];

const int screen_width = 1280;
const int screen_height = 768;
// const int screen_bpp = 32;
const int FPS = 60;

const int color_key_rgb = 180;

#define UI_BAR_THICHNESS 5

#define blank_tile0 0

#define object_tile3 3
#define object_tile4 4
#define object_tile5 5
#define object_tile7 7
#define object_tile8 8
#define object_tile9 9
#define object_tile10 10
#define tile_size 64

#define max_home_map_x 20
#define max_home_map_y 12
#define max_level_map_x 60
#define max_level_map_y 36

struct input{
    int left;
    int right;
    int up;
    int down;
};

struct map{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[max_level_map_y][max_level_map_x];
    std::string file_name;
};

namespace SDLCommonFunc{
    bool checkTileCollision(int tile);
    bool checkCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    bool checkFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
    float calAngle(const int& xm,const int& ym,const int& x_screen_pos,const int& y_screen_pos);
    int showMenu(SDL_Renderer* des, TTF_Font* font, TTF_Font* hfont);
    int handleMenu(SDL_Event events, SDL_Rect pauseIcon);
    int showPause(SDL_Renderer* des, TTF_Font* font, TTF_Font* hfont);
    int showHelp(SDL_Renderer* des, TTF_Font* font);
    int showVictory(SDL_Renderer* des, TTF_Font* font, std::string name, UINT mark_value);
}

struct geometricFormat{
    public:
        geometricFormat(int tleft, int ttop, int twidth, int theight){ left = tleft, top= ttop, width = twidth, height = theight;}
        int left;
        int top;
        int width;
        int height;
};

struct colorData{
        colorData(Uint8 r, Uint8 g, Uint8 b){red = r, green = g, blue= b;}
    public:
        Uint8 red;
        Uint8 green;
        Uint8 blue;
};

// #endif