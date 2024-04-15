#include "headers/textObject.h"

textObject::textObject(){
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;
    texture = NULL;
}

textObject::~textObject(){
    free();
}

bool textObject::loadFromRenderText(TTF_Font* font, SDL_Renderer* screen){
    free();
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if(text_surface){
        texture = SDL_CreateTextureFromSurface(screen, text_surface);
        width = text_surface->w;
        height = text_surface->h;
        SDL_FreeSurface(text_surface);
    }
    return texture != NULL;
}

void textObject::free(){
    if(texture != NULL){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void textObject::setColor(Uint8 red, Uint8 green, Uint8 blue){
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}

void textObject::setColorType(int type){
    if(type == RED){
        SDL_Color color = {255, 0 ,0};
        text_color = color;
    } else if(type == WHITE){
        SDL_Color color = {255, 255,255};
        text_color = color;
    } else if(type == BLACK){
        SDL_Color color = {0, 0 ,0};
        text_color = color;
    }
}

void textObject::renderText(SDL_Renderer* screen, int x_pos, int y_pos, SDL_Rect* clip /*= NULL*/,
                        double angle/* =0.0*/, SDL_Point* center/* = NULL*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/){
    SDL_Rect renderQuad = {x_pos, y_pos, width, height};
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(screen, texture, clip, &renderQuad, angle, center, flip);
}