// #ifndef GAME_MAP_H
// #define GAME_MAP_H
#pragma once
#include "commonFunc.h"
#include "baseObject.h"

#define max_tiles 30

class tileMat : public baseObject{
    public:
        tileMat() {;}
        ~tileMat() {;}
};

class gameMap{
    public:
        gameMap(){;}
        ~gameMap(){;}

        void loadMap(std::string name);
        void loadTiles(SDL_Renderer* screen);
        void drawMap(SDL_Renderer* screen);
        map getMap() const {return game_map;};
        void setMap(map& map_data) {game_map = map_data;};
    private:
        map game_map;
        tileMat tile_mat[max_tiles];
};

// #endif