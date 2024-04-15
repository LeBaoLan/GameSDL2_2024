#pragma once

#include "commonFunc.h"
#include "baseObject.h"

class textObject : public baseObject
{
    public:
        textObject();
        ~textObject();

        enum textColor{
            RED = 0,
            WHITE = 1,
            BLACK = 2,
        };

        bool loadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
        void free();

        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setColorType(int type);

        void renderText(SDL_Renderer* screen, int x_pos, int y_pos, SDL_Rect* clip = NULL,
                        double angle =0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
                        // =show
        int getWidth() const {return width;}
        int getHeight() const {return height;}

        std::string getText() const {return str_val;}
        void setText(const std::string& text) {str_val = text;}

        // void createGameText(TTF_Font* font, SDL_Surface* des){
        //     p_surface_object = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
        //     show(des);
        // } // for game menu
    private:
        std::string str_val;
        SDL_Color text_color;
        SDL_Texture* texture;
        int width;
        int height;
};
