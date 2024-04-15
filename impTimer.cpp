#include "headers/impTimer.h"
#include "headers/commonFunc.h"

impTimer::impTimer(){
    start_tick=0;
    paused_tick=0;
    is_paused = false;
    is_started = false;
}

impTimer::~impTimer(){

}

void impTimer::start(){
    is_started = true;
    is_paused = false;
    start_tick = SDL_GetTicks();
}

void impTimer::stop(){
    is_paused = false;
    is_started = false;
}

void impTimer::paused(){
    if(is_started == true && is_paused == false){
        is_paused = true;
        paused_tick = SDL_GetTicks() - start_tick;
    }
}

void impTimer::unpaused(){
    if(is_paused == true){
        is_paused = false;
        start_tick = SDL_GetTicks() - paused_tick;
        paused_tick = 0;
    }
}

int impTimer::get_ticks(){
    if(is_started == true){
        if(is_paused == true){
            return paused_tick;
        } else {
            return SDL_GetTicks() - start_tick;
        }
    }

    return 0;
}

bool impTimer::check_is_started(){
    return is_started;
}

bool impTimer::check_is_paused(){
    return is_paused;
}