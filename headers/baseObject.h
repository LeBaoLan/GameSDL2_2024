// #ifndef BASE_OBJECT_H
// #define BASE_OBJECT_H
#pragma once

#include "commonFunc.h"

class baseObject{
    public:
        baseObject();
        ~baseObject();
        void setRect(const int& x, const int &y) {rect.x =x, rect.y =y;}
        void setRectWH(const int& w, const int &h) {rect.w =w, rect.h =h;}
        SDL_Rect getRect() const {return rect;}
        SDL_Texture* getObject() const {return p_object;}

        virtual bool loadIMG(std::string path, SDL_Renderer* screen);
        void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void free();
    protected:
        SDL_Texture* p_object;
        SDL_Rect rect;
};
// #endif