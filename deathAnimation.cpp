#include "headers/deathAnimation.h"

deathAnimation::deathAnimation(){
    frame_height = 0;
    frame_width = 0;
    frame = 0;
    animation_loop = 0;
}

deathAnimation::~deathAnimation(){

}

bool deathAnimation::loadIMG(std::string path, SDL_Renderer* screen){
    bool ret = baseObject::loadIMG(path, screen);
    if(ret){
        frame_width = rect.w/DEATH_FRAME_NUM;
        frame_height = rect.h;
    }
    return ret;
}

void deathAnimation::set_clip(){
    if(frame_width >0 && frame_height >0){
        for(int i=0; i<DEATH_FRAME_NUM; i++){
        frame_clip[i].x = i*frame_width;
        frame_clip[i].y = 0;
        frame_clip[i].w = frame_width;
        frame_clip[i].h = frame_height;
        }
    }
}

void deathAnimation::set_frame(const int& fr){
    frame = fr;
}

void deathAnimation::show(SDL_Renderer* screen){
    SDL_Rect* current_clip = &frame_clip[frame];
    rect.x = x_pos - map_x;
    rect.y = y_pos - map_y;
    if(frame >= DEATH_FRAME_NUM)
    {
        frame = 0;
        animation_loop ++ ;
    }
    SDL_Rect RenderQuard = {rect.x,rect.y,frame_width,frame_height};
    SDL_RenderCopy(screen, p_object, current_clip, &RenderQuard);
    if(d_frame_delay > 3){
        frame++;
        d_frame_delay=0;
    } else {
        d_frame_delay++;
    }

    // SDL_Rect* current_clip = &frame_clip[frame];
    // SDL_Rect render_quad = {rect.x, rect.y, frame_width, frame_height};
    // if(current_clip != NULL){
    //     render_quad.w = current_clip->w;
    //     render_quad.h = current_clip->h;
    // }
    // SDL_RenderCopy(screen, p_object, current_clip, &render_quad);
}