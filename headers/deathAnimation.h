#pragma once

#include "commonFunc.h"
#include "baseObject.h"

#define DEATH_FRAME_NUM 8

class deathAnimation : public baseObject{
    public:
        deathAnimation();
        ~deathAnimation();

        void set_clip();
        void set_frame(const int& fr);
        virtual bool loadIMG(std::string path, SDL_Renderer* screen);
        void show(SDL_Renderer* screen);
        int get_frame_width() const {return frame_width;}
        int get_frame_height() const {return frame_height;}
        
        void set_x_val(const float& xVal){x_val = xVal;}
        void set_y_val(const float& yVal){y_val = yVal;}
        void set_x_pos(const float& xPos){x_pos = xPos;}
        void set_y_pos(const float& yPos){y_pos = yPos;}

        float get_x_val() const {return x_val;}
        float get_y_val() const {return y_val;}
        float get_x_pos() const {return x_pos;}
        float get_y_pos() const {return y_pos;}

        void setMapXY(const int& mp_x,const int& mp_y){map_x = mp_x,map_y = mp_y;}

        int get_animation_loop() const {return animation_loop;}

        int d_frame_delay = 0;
    private:
        int frame_width;
        int frame_height;
        float x_pos;
        float y_pos;
        float x_val;
        float y_val;
        int map_x;
        int map_y;
        int animation_loop;

        unsigned int frame;
        SDL_Rect frame_clip[DEATH_FRAME_NUM];
};