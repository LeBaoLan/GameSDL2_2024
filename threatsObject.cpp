#include <iostream>
#include "headers/threatsObject.h"
#include "headers/impTimer.h"

threatsObject::threatsObject(){
    width_frame = 0;
    height_frame = 0;
    x_val = 0.0;
    y_val = 0.0;
    x_pos = 0.0;
    y_pos = 0.0;
    x_des = 0.0;
    y_des = 0.0;
    frame = 0;


    input_type.left = 0;
    input_type.right = 0;
    enemy_type = 0;
    health = 0;

    alerted = false;
    latest = -1;
    randDelay = 499;
    restTime = 0;
    recentlyHit = 0;
}

threatsObject::~threatsObject(){

}

bool threatsObject::loadIMG(std::string path, SDL_Renderer* screen){
    bool ret = baseObject::loadIMG(path,screen);
    if(ret){
        width_frame = rect.w/THREAT_FRAME_NUM;
        height_frame = rect.h;
    }
    return ret;
}

void threatsObject::set_clip(){
    if (width_frame > 0 && height_frame > 0){
        for(int i=0; i<THREAT_FRAME_NUM; i++){
        frame_clip[i].x = i*width_frame;
        frame_clip[i].y = 0;
        frame_clip[i].w = width_frame;
        frame_clip[i].h = height_frame;
        }
    }
}

void threatsObject::hit_enemy(){
    g_sound_effects[1] = Mix_LoadWAV("sounds//sound_effects//flesh_hit.wav");
    Mix_Volume(-1, 32);
    Mix_PlayChannel(2, g_sound_effects[1], 0);
    health -= 20;
    recentlyHit=1;
}

void threatsObject::frame_delayer(){
    if(frame_delay > 5){
        frame++;
        frame_delay= 0;
    } else {
        frame_delay++;
    }
}

void threatsObject::show(SDL_Renderer* des){
    rect.x = x_pos - map_x;
    rect.y = y_pos - map_y;
    frame_delayer();
    if(frame >= THREAT_FRAME_NUM){
        frame = 0;
    }
    SDL_Rect* currentClip = &frame_clip[frame];
    SDL_Rect rendQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_RenderCopy(des, p_object, currentClip, &rendQuad);
}

SDL_Rect threatsObject::getRectFrame(){
    SDL_Rect tmp_rect;
    tmp_rect.x = rect.x;
    tmp_rect.y = rect.y;
    tmp_rect.w = width_frame;
    tmp_rect.h = height_frame;
    return tmp_rect;
}

void threatsObject::impIMGType(SDL_Renderer* screen){
    int type = this->get_enemy_type();
    bool tRet = true;
    if(type == PUMPKIN){
        if(input_type.left == 1){
            loadIMG("img//Pumpkin//Pumpkin_jump_left", screen);
            tRet = this->loadIMG("img//Pumpkin//Pumpkin_jump_left.png", screen);
            input_type.right = 0;
            latest = move_left;
        } else if(input_type.right == 1){
            loadIMG("img//Pumpkin//Pumpkin_jump_right", screen);
            tRet = this->loadIMG("img//Pumpkin//Pumpkin_jump_right.png", screen);
            input_type.left = 0;
            latest = move_right;
        } else if(input_type.right==0 && input_type.left==0 && input_type.up==0 && input_type.down==0){
            if(latest == move_left){
                loadIMG("img//Pumpkin//Pumpkin_Idle_left", screen);
                tRet = this->loadIMG("img//Pumpkin//Pumpkin_Idle_left.png", screen);
            } else if(latest == move_right) {
                loadIMG("img/Pumpkin//Pumpkin_Idle_right", screen);
                tRet = this->loadIMG("img//Pumpkin//Pumpkin_Idle_right.png", screen);
            }
        }
    } else if (type == FROG){
        if(input_type.left == 1){
            loadIMG("img//ToxicFrog//ToxicFrog_Hop_left", screen);
            tRet = this->loadIMG("img//ToxicFrog//ToxicFrog_Hop_left.png", screen);
            input_type.right = 0;
            latest = move_left;
        } else if(input_type.right == 1){
            loadIMG("img//ToxicFrog//ToxicFrog_Hop_right", screen);
            tRet = this->loadIMG("img//ToxicFrog//ToxicFrog_Hop_right.png", screen);
            input_type.left = 0;
            latest = move_right;
        } else if(input_type.right==0 && input_type.left==0 && input_type.up==0 && input_type.down==0){
            if(latest == move_left){
                loadIMG("img//ToxicFrog//ToxicFrog_Idle_left", screen);
                tRet = this->loadIMG("img//ToxicFrog//ToxicFrog_Idle_left.png", screen);
            } else if(latest == move_right) {
                loadIMG("img//ToxicFrog//ToxicFrog_Idle_right", screen);
                tRet = this->loadIMG("img//ToxicFrog//ToxicFrog_Idle_right.png", screen);
            }
        }
    }
    if(tRet == false){
        std::cout << "failed to load an enemy: " << SDL_GetError() << std::endl;
    }
}

void threatsObject::idling(SDL_Renderer* screen, int& desX, int& desY, int pl_pos_x, int pl_pos_y){
    if(abs(x_pos - pl_pos_x)<5*tile_size && abs(x_pos - spawn_x_pos)< 5*tile_size
    && abs(y_pos - pl_pos_y)<5*tile_size && abs(y_pos - spawn_y_pos)< 5*tile_size){
        alerted = true;
    }

    if(recentlyHit == 1){
        alerted = true;
        recentlyHit = 0;
    }

    if(abs(x_pos - spawn_x_pos)> 15*tile_size || abs(y_pos - spawn_y_pos)> 15*tile_size
    || sqrt((x_pos-pl_pos_x)*(x_pos-pl_pos_x) + (y_pos-pl_pos_y)*(y_pos-pl_pos_y)) > 10*tile_size){
        alerted = false;
        set_x_des(spawn_x_pos);
        set_y_des(spawn_y_pos);
        etime = 0;
    }

    if(alerted == false){
        if(randDelay > 500){
            
            set_x_des(spawn_x_pos+desX*tile_size);
            set_y_des(spawn_y_pos+desY*tile_size);
            // std::cout << desX << " " << desY << std::endl;

            randDelay = 0;
        } else {
            randDelay++;
        }
    } else if(alerted == true){
        if(this->get_enemy_type() == FROG){
            if(restTime>=100){
                projectile* p_bullet = new projectile();
                this->initBullet(p_bullet, screen);
                restTime=0;
            } else {
                restTime++;
            }
        }
        baseObject emoji;
        emoji.loadIMG("img//Emoji//SHOCK.png", screen);
        // emoji.setRect(x_pos-16, y_pos-16);
        // emoji.setRectWH(16, 16);
        SDL_Rect erenderquad = {rect.x-16, rect.y-16, 16, 16};
        if(etime < 50){
            SDL_RenderCopy(screen, emoji.getObject(), NULL, &erenderquad);
            etime++;
        }
        set_x_des(pl_pos_x);
        set_y_des(pl_pos_y);
    }
}

void threatsObject::doThreat(map& gMap){
    x_val = 0;
    y_val = 0;
    if(abs(x_pos - x_des)>1){
        if(x_pos<x_des){
            x_val += THREAT_SPEED;
            input_type.left = 0;
            input_type.right = 1;
        } else if(x_pos>x_des){
            x_val -= THREAT_SPEED;
            input_type.left = 1;
            input_type.right = 0;
        }
    } else {
        x_val=0;
        input_type.left = 0;
        input_type.right = 0;
    }
    if(abs(y_pos - y_des)>1){
        if(y_pos<y_des){
            y_val += THREAT_SPEED;
            input_type.up = 0;
            input_type.down = 1;
        } else if(y_pos>y_des){
            y_val -= THREAT_SPEED;
            input_type.up = 1;
            input_type.down = 0;
        }
    } else {
        y_val=0;
        input_type.up = 0;
        input_type.down = 0;
    }

    checkToMap(gMap);
}

void threatsObject::checkToMap(map& map_data){
    int x1 = 0;
    int x2 = 0;
    
    int y1 = 0;
    int y2 = 0;

    //Check horizontal
    int height_min = height_frame < tile_size ? height_frame : tile_size;

    x1 = (x_pos + 2 + x_val)/tile_size;
    x2 = (x_pos - 2 + x_val + width_frame)/tile_size;

    y1 = (y_pos + 2)/tile_size;
    y2 = (y_pos - 2 + height_min)/tile_size;

    // std::cout << x1 << " " << x2 << std::endl << y1 << " " << y2 << std::endl;

    if(x1>=0 && x2<max_level_map_x && y1>=0 && y2<max_level_map_y){
        if(x_val > 0){
            //mainProtagonist is moving to the right
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y1][x2])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y2][x2]) ){
                x_pos = x2*tile_size;
                x_pos -= (width_frame + 1);
                x_val = 0;
            }
        } else if(x_val < 0){
            //is moving to the left
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y1][x1])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y2][x1]) ){
                x_pos = (x1 + 1)*tile_size;
                x_val = 0;
            }
        }
    }

    //Check vertical
    int width_min = width_frame < tile_size ? width_frame : tile_size;
    x1 = (x_pos + 2)/tile_size;
    x2 = (x_pos - 2 + width_min)/tile_size;

    y1 = (y_pos + 2 + y_val)/tile_size;
    y2 = (y_pos - 2 + y_val + height_frame)/tile_size;

    if(x1>=0 && x2<max_level_map_x && y1>=0 && y2<max_level_map_y){
        if(y_val > 0){
            //is going down
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y2][x1])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y2][x2]) ){
                y_pos = y2*tile_size;
                y_pos -= (height_frame + 1);
                y_val = 0;
            }
        } else if(y_val < 0){
            //is going up
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y1][x1])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y1][x2]) ){
                y_pos = (y1 + 1)*tile_size;
                y_val = 0;
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;
    // std::cout << "---------" << std::endl;
    // std::cout << x_val << " " << y_val << std::endl;
    // std::cout << x_pos << " " << y_pos << std::endl;
    
    if(x_pos < 0){
        x_pos = 0;
    } else if(x_pos + width_frame > map_data.max_x){
        x_pos = map_data.max_x - width_frame - 1;
    }
    if(y_pos < 0){
        y_pos = 0;
    } else if(y_pos + height_frame > map_data.max_y){
        y_pos = map_data.max_y - height_frame - 1;
    }
}

void threatsObject::initBullet(projectile* p_bullet, SDL_Renderer* screen){
    if(p_bullet != NULL){
        p_bullet->set_bullet_type(projectile::FROG_SALIVA);
        bool ret = p_bullet->loadImgBullet(screen);
        
        if(ret){
            p_bullet->set_is_move(true);
            // load arrow
            p_bullet->loadImgBullet(screen);
            p_bullet->set_bullet_dir(projectile::MAIN_MULTI_DIR);
            
            p_bullet->setRect(this->rect.x + width_frame/2, this->rect.y + height_frame/2);
            t_bullet_list.push_back(p_bullet);
        }
    }
}

void threatsObject::makeBullet(SDL_Renderer* screen, float pl_x_pos, float pl_y_pos){
    for(int i=0; i<t_bullet_list.size(); i++){
        projectile* p_bullet = t_bullet_list.at(i);
        if(p_bullet != NULL){
            if(p_bullet->get_is_move() == true && alerted == true){
                if(p_bullet->get_bullet_type() == projectile::FROG_SALIVA){
                    float angle = SDLCommonFunc::calAngle(pl_x_pos, pl_y_pos, x_pos, y_pos);
                    float radAngle = angle * (1.00 * M_PI/180);
                    float b_x_val = 1.00 * (BULLET_SPEED*cos(radAngle));
                    float b_y_val = 1.00 * (BULLET_SPEED*sin(radAngle));
                    p_bullet->set_x_val(b_x_val);
                    p_bullet->set_y_val(b_y_val);
                    int bullet_distance= rect.x + width_frame - p_bullet->getRect().x;

                    if(abs(bullet_distance)<600){
                        p_bullet->handleMove(screen_width, screen_height);
                        p_bullet->renderArrow(screen, angle);
                    } else {
                        p_bullet->set_is_move(false);
                    }
                }
            } else {
                t_bullet_list.erase(t_bullet_list.begin() + i);
                if(p_bullet != NULL){
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void threatsObject::removeBullet(const int& idx){
    int size = t_bullet_list.size();
    if(size > 0 && idx < size){
        projectile* p_bullet = t_bullet_list.at(idx);
        t_bullet_list.erase(t_bullet_list.begin() + idx);
        if(p_bullet){
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}
