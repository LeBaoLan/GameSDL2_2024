#include "headers/baseObject.h"

baseObject::baseObject(){
    p_object = NULL;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
}

baseObject::~baseObject(){
    free();
}

bool baseObject::loadIMG(std::string path, SDL_Renderer* screen){
    free();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL){
        //Set background color for transparent
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, color_key_rgb, color_key_rgb, color_key_rgb));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if(new_texture != NULL){
            rect.w = load_surface->w;
            rect.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    p_object = new_texture;
    return p_object != NULL;
}

void baseObject::render(SDL_Renderer* des, const SDL_Rect* clip /* = NULL*/){
    SDL_Rect renderquad = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderCopy(des, p_object, clip, &renderquad);
}

void baseObject::free(){
    if(p_object != NULL){
        SDL_DestroyTexture(p_object);
        p_object = NULL;
        rect.w = 0;
        rect.h = 0;
    }
}
