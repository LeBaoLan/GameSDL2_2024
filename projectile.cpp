#include "headers/projectile.h"

projectile::projectile(){
    p_x_val = 0;
    p_y_val = 0;
    is_move = false;
    bullet_type = TMP_MAIN_BULLET;
}

projectile::~projectile(){

}

void projectile::handleMove(const int& x_border, const int& y_border){
    // rect.x = x_pos - map_x;
    // rect.y = y_pos - map_y;
    if(bullet_dir == DIR_RIGHT){
        rect.x += p_x_val;
        if(rect.x > x_border){
            is_move = false;
        }
    } else if (bullet_dir == DIR_LEFT){
        rect.x -= p_x_val;
        if(rect.x < 0){
            is_move = false;
        }
    } else if(bullet_dir == MAIN_MULTI_DIR){
        rect.x+=p_x_val;
        rect.y+=p_y_val;
        if(rect.x > x_border || rect.x < 0){
            is_move = false;
        }
        if(rect.y > y_border || rect.y < 0){
            is_move = false;
        }
    }
}

bool projectile::loadImgBullet(SDL_Renderer* des){
    bool ret = false;
    if(bullet_type == FROG_SALIVA){
        ret = loadIMG("img//frog_saliva.png", des);
    } else if (bullet_type == MAIN_ARROW){
        ret = loadIMG("img//Arrow.png", des);
    } else {
        ret = loadIMG("img//bullet.png", des);
    }
    return ret;
}

void projectile::renderArrow(SDL_Renderer* des, float angle){
    const SDL_Rect* clip = NULL;
    SDL_Rect renderQuad = {rect.x-16, rect.y, rect.w, rect.h};
    SDL_RenderCopyEx(des, p_object, clip, &renderQuad, angle, NULL, SDL_FLIP_NONE);
}