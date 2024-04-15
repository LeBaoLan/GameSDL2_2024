#include "headers/attackAction.h"

attackAction::attackAction(){
    frame =0;
    width_frame = 0;
    height_frame = 0;
}

attackAction::~attackAction(){

}

bool attackAction::loadIMG(std::string path, SDL_Renderer* screen){
    bool ret = baseObject::loadIMG(path, screen);
    if(ret == true){
        width_frame = rect.w/8;
        height_frame = rect.h;
    } else {
        std::cout << "fail to load attack: " << SDL_GetError() << std::endl;
    }
    return ret;
}



void attackAction::set_clip(){
    if(width_frame >0 && height_frame >0){
        
        for(int i=0; i<8; i++){
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }
}

void attackAction::render(SDL_Renderer* des, float angle){
    SDL_Rect* b_current_clip = &frame_clip[frame];
    if(frame<7){
        if(frame_delay > 3){
            frame++;
            frame_delay=0;
        } else {
            frame_delay++;
        }
    }
    SDL_Point pt;
    pt.x = 16;
    pt.y = 24;
    SDL_Rect renderQuad = {rect.x+16, rect.y+16, width_frame, height_frame};
    SDL_RenderCopyEx(des, p_object, b_current_clip, &renderQuad, angle, &pt, SDL_FLIP_NONE);
}