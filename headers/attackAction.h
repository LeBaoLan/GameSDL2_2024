#pragma once

#include "baseObject.h"


class attackAction : public baseObject
{
    public:
        attackAction();
        ~attackAction();
    
        bool loadIMG(std::string path, SDL_Renderer* screen);
        void set_clip();
        int getCurrentFrame(){ return frame;}
        void plusFrame(){frame++;}
        void setWidth(int const a) {frame = a;}
        void setHeight(int const a) {frame = a;}
        int getWidth() {return frame;}
        int getHeight() {return frame;}

        SDL_Rect getFrameClip(int f){return frame_clip[f];}
        void setCurrentFrame(int const f) {frame = f;}
        void resetChargingFrame(){frame = 0;}
        
        void render(SDL_Renderer* des, float angle);
        int frame_delay = 0;
    private:
        int width_frame;
        int height_frame;

        SDL_Rect frame_clip[8];
        int frame;
};