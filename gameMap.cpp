#include <fstream>
#include <iostream>

#include "headers/gameMap.h"

void gameMap::loadTiles(SDL_Renderer* screen){
    char file_img[30];
    FILE* fp = NULL;

    for(int i=0; i< max_tiles; i++){
        sprintf_s(file_img, "load_map_img/%d.png", i);

        fopen_s(&fp, file_img, "rb");
        if(fp == NULL){
            continue;
        }
        fclose(fp);

        tile_mat[i].loadIMG(file_img, screen);
    }
}

void gameMap::drawMap(SDL_Renderer* screen){
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    int map_x=0;
    int map_y=0;

    map_x = game_map.start_x/tile_size;

    x1 = (game_map.start_x%tile_size)*-1;
    x2 = x1 + screen_width + (x1==0 ? 0 : tile_size);

    map_y = game_map.start_y/tile_size;

    y1 = (game_map.start_y%tile_size)*-1;
    y2 = y1 + screen_height + (y1==0 ? 0 : tile_size);

    for(int i=y1; i<y2; i+= tile_size){

        map_x = game_map.start_x/tile_size;

        for(int j=x1; j<x2; j+= tile_size){
            int val = game_map.tile[map_y][map_x];

            if(val>=0){
                tile_mat[val].setRect(j,i);
                tile_mat[val].render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}

void gameMap::loadMap(std::string name)
{
    std::ifstream file(name);
    if (!file.is_open())
    {
        std:: cout << "Couldn't open file: " << name << ", specificly: " << SDL_GetError() << std::endl;
        return;
    }

    game_map.max_x = 0;
    game_map.max_y = 0;
    for (int i = 0; i < max_level_map_y; i++)
    {
        for (int j = 0; j < max_level_map_x; j++)
        {
            file >> game_map.tile[i][j];
            int val = game_map.tile[i][j];
            if (val > 0)
            {
                if (j > game_map.max_x)
                {
                    game_map.max_x = j;
                }

                if (i > game_map.max_y)
                {
                    game_map.max_y = i;
                }
            }
        }
    }
    game_map.max_x = (game_map.max_x + 1) * tile_size;
    game_map.max_y = (game_map.max_y + 1) * tile_size;

    game_map.start_x = 0;
    game_map.start_y = 0;

    game_map.file_name = name;

    file.close();
}