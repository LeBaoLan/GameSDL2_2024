#include <cmath>
#include "headers/mainProtagonist.h"

mainProtagonist::mainProtagonist(){
    frame =0;
    x_pos =1920;
    y_pos =2240;
    x_val =0;
    y_val =0;
    width_frame =0;
    height_frame =0;
    status =-1;
    input_type.left=0;
    input_type.right=0;
    input_type.up=0;
    input_type.down=0;
    map_x=0;
    map_y=0;
    is_charging_bow = false;
    x_screen_pos=screen_width/2;
    y_screen_pos=screen_height/2;
    key = 0;
    health = 100;
    bow_charge = 0;
    ammo=50;
    come_back_time = 100;
}

mainProtagonist::~mainProtagonist(){

}

bool mainProtagonist::loadIMG(std::string path, SDL_Renderer* screen){
    bool ret = baseObject::loadIMG(path, screen);
    if(ret == true){
        width_frame = rect.w/8;
        height_frame = rect.h;
    }
    return ret;
}

void mainProtagonist::set_clip(){
    if(width_frame >0 && height_frame >0){
        
        for(int i=0; i<8; i++){
        frame_clip[i].x = i*width_frame;
        frame_clip[i].y = 0;
        frame_clip[i].w = width_frame;
        frame_clip[i].h = height_frame;
        }
    }
}

void mainProtagonist::frame_delayer(){
    if(frame_delay > 5){
        frame++;
        frame_delay= 0;
    } else {
        frame_delay++;
    }
}

void mainProtagonist::show(SDL_Renderer* des){
    if (status == walk_left){
        loadIMG("img//Main_player//Main_player_run_left.png", des);  
        latest = walk_left;
    } else if(status == walk_right){
        loadIMG("img//Main_player//Main_player_run_right.png", des);
        latest = walk_right;
    } else {
        status = latest;
    }

    if(input_type.left == 1 || input_type.right == 1 || input_type.up == 1 || input_type.down == 1){
        frame_delayer();
        g_sound_effects[3] = Mix_LoadWAV("sounds//sound_effects//step_grass.wav");
        Mix_Volume(-1, 32);
        Mix_PlayChannel(4, g_sound_effects[3], 0);
    } else {
        if(status == walk_left){
            loadIMG("img//Main_player//Main_player_idle_left.png", des);
            frame_delayer();
        } else if(status == walk_right){
            loadIMG("img//Main_player//Main_player_idle_right.png", des);
            frame_delayer();
        }
    }
    if(frame >=8){
        frame = 0;
    }

    rect.x = x_pos - map_x;
    rect.y = y_pos - map_y;

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_RenderCopy(des, p_object, current_clip, &renderQuad);
}

void mainProtagonist::got_hit(SDL_Renderer* screen){
    health -= 10;
    is_charging_bow = false;
    if(health > 0){
        g_sound_effects[7] = Mix_LoadWAV("sounds//sound_effects//p_impact_flesh.wav");
        Mix_Volume(-1, 128);
        Mix_PlayChannel(-1, g_sound_effects[7], 0);
    }
}

//#include <iostream>
void mainProtagonist::handleInputAction(SDL_Event events, SDL_Renderer* screen, int &xm, int &ym, map &map_data, int& gameState){
    if(events.type == SDL_KEYDOWN){
        switch (events.key.keysym.sym)
        {
            case SDLK_d:
                {
                    if(is_charging_bow == false){
                        status = walk_right;
                    }
                    input_type.right = 1;
                    input_type.left = 0;
                    //std::cout << "right on" << std::endl;
                }
                break;
            case SDLK_a:
                {
                    if(is_charging_bow == false){
                        status = walk_left;
                    }
                    input_type.left = 1;
                    input_type.right = 0;
                    //std::cout << "left on" << std::endl;
                }
                break;
            case SDLK_w:
                {
                    input_type.up = 1;
                    input_type.down = 0;
                    //std::cout << "up on" << std::endl;
                }
                break;
            case SDLK_s:
                {
                    input_type.down = 1;
                    input_type.up = 0;
                    //std::cout << "down on" << std::endl;
                }
                break;
            case SDLK_f:
                {
                    int x1 = (x_pos - 5)/tile_size;
                    int x2 = (x_pos + width_frame + 5)/tile_size;

                    int y1 = (y_pos - 5)/tile_size;
                    int y2 = (y_pos + height_frame + 5)/tile_size;
                    if(map_data.tile[y1][x1] == object_tile8 || map_data.tile[y1][x2] == object_tile8
                    || map_data.tile[y2][x1] == object_tile8 || map_data.tile[y2][x2] == object_tile8){
                        key++;
                        ammo+=10;
                        if(map_data.tile[y1][x1] == object_tile8){
                            map_data.tile[y1][x1] = 0;
                        } else if(map_data.tile[y1][x2] == object_tile8){
                            map_data.tile[y1][x2] = 0;
                        } else if(map_data.tile[y2][x1] == object_tile8){
                            map_data.tile[y2][x1] = 0;
                        } else if(map_data.tile[y2][x2] == object_tile8){
                            map_data.tile[y2][x2] = 0;
                        }
                    }
                    if( (map_data.tile[y1][x1] == object_tile10 || map_data.tile[y1][x2] == object_tile10
                    || map_data.tile[y2][x1] == object_tile10 || map_data.tile[y2][x2] == object_tile10)
                    && key == 4){
                        gameState = 2;//win
                    }
                }
                break;
        }
    }
    else if (events.type == SDL_KEYUP){
        switch (events.key.keysym.sym)
        {
            case SDLK_d:
                {
                    input_type.right = 0;
                    //std::cout << "right off" << std::endl;
                }
                break;
            case SDLK_a:
                {
                    input_type.left = 0;
                    //std::cout << "left off" << std::endl;
                }
                break;
            case SDLK_w:
                {
                    input_type.up = 0;
                    //std::cout << "up off" << std::endl;
                }
                break;
            case SDLK_s:
                {
                    input_type.down = 0;
                    //std::cout << "down off" << std::endl;
                }
                break;
        }
    }

    if(events.type == SDL_MOUSEBUTTONDOWN){
        // if (events.button.button == SDL_BUTTON_RIGHT){
        //     projectile* p_bullet = new projectile();
        //     p_bullet->set_bullet_type(projectile::TMP_MAIN_BULLET);
        //     p_bullet->loadImgBullet(screen);
            
        //     if(status == walk_left){
        //         p_bullet->set_bullet_dir(projectile::DIR_LEFT);
        //         p_bullet->setRect(this->rect.x + 20, rect.y + height_frame*0.3);//pos x and pos y (how deep and high the bullet spawn)
        //     } else if (status == walk_right){
        //         p_bullet->set_bullet_dir(projectile::DIR_RIGHT);
        //         p_bullet->setRect(this->rect.x + width_frame - 20, rect.y + height_frame*0.3);
        //     }

        //     p_bullet->set_x_val(20);
        //     p_bullet->set_is_move(true);

        //     bullet_list.push_back(p_bullet);
            
        // }
        if (events.button.button == SDL_BUTTON_LEFT){
            is_charging_bow =true;
        }
    }
    if(events.type == SDL_MOUSEBUTTONUP){
        if (events.button.button == SDL_BUTTON_LEFT && bow_charge == maxium_charging && is_charging_bow == true){
            g_sound_effects[0] = Mix_LoadWAV("sounds//sound_effects//bow_shoot.wav");
            Mix_Volume(-1, 32);
            Mix_PlayChannel(1, g_sound_effects[0], 0);
            is_charging_bow =false;
            bow_charge = 0;
            ammo--;
            int tempx = x_screen_pos;
            int tempy = y_screen_pos;
            float angle = SDLCommonFunc::calAngle(xm, ym, tempx, tempy);
            // load arrow
            projectile* p_bullet = new projectile();
            p_bullet->set_bullet_type(projectile::MAIN_ARROW);
            p_bullet->loadImgBullet(screen);

            p_bullet->set_bullet_dir(projectile::MAIN_MULTI_DIR);

            float radAngle = angle * (1.00 * M_PI/180);
            float b_x_val = 1.00 * (20*cos(radAngle));
            float b_y_val = 1.00 * (20*sin(radAngle));
            // this cal with rad already has positive and negative
            
            p_bullet->setRect(this->rect.x + width_frame/2, this->rect.y + height_frame/2);

            p_bullet->set_x_val(b_x_val);
            p_bullet->set_y_val(b_y_val);
            p_bullet->set_is_move(true);

            bullet_list.push_back(p_bullet);
        } else if (events.button.button == SDL_BUTTON_LEFT && bow_charge < maxium_charging){
            is_charging_bow =false;
            bow_charge = 0;
        }
    }
}

void mainProtagonist::handleAttacksAction(SDL_Renderer* screen, attackAction& p_bow, int& xm, int& ym){
    if(is_charging_bow == true){
        if(bow_charge < maxium_charging){
            bow_charge++;
        }
        if(xm < x_screen_pos){
            status = walk_left;
        } else {
            status = walk_right;
        }

        int tempx = x_screen_pos;
        int tempy = y_screen_pos;
        float angle = SDLCommonFunc::calAngle(xm, ym, tempx, tempy);
        p_bow.setRect(this->getRect().x, this->getRect().y);
        p_bow.render(screen, angle);
    } else if(is_charging_bow == false){
        p_bow.resetChargingFrame();
    }
}

void mainProtagonist::initHealth(SDL_Renderer* screen){
    baseObject* first_health_bar = new baseObject[1];
    first_health_bar->loadIMG("img//Bar//first_bar.png", screen);
    bool fRet = first_health_bar->loadIMG("img//Bar//first_bar.png", screen);
    if(!fRet) {
        first_health_bar->free();
        return;
    }
    first_health_bar->setRect(106, 38);
    first_health_bar->setRectWH(15, 30);
    healthBar.push_back(first_health_bar);

    baseObject* mid_health_bar = new baseObject[8];
    for(int i=0; i<8; i++){
        baseObject* mid_bar = (mid_health_bar + i);
        mid_bar->loadIMG("img//Bar//mid_bar.png", screen);
        bool mRet = mid_bar->loadIMG("img//Bar//mid_bar.png", screen);
        if(!mRet) {
            mid_bar->free();
            return;
        }
        mid_bar->setRect((106+15+15*i), 38);
        mid_bar->setRectWH(15, 30);
        healthBar.push_back(mid_bar);
    }
    
    baseObject* end_health_bar = new baseObject[1];
    end_health_bar->loadIMG("img//Bar//end_bar.png", screen);
    bool eRet = end_health_bar->loadIMG("img//Bar//end_bar.png", screen);
    if(!eRet) {
        end_health_bar->free();
        return;
    }
    end_health_bar->setRect((106+135), 38);
    end_health_bar->setRectWH(15, 30);
    healthBar.push_back(end_health_bar);

    baseObject* first_ammo_bar = new baseObject[1];
    first_ammo_bar->loadIMG("img//Bar//first_ammo.png", screen);
    bool bfRet = first_ammo_bar->loadIMG("img//Bar//first_ammo.png", screen);
    if(!fRet) {
        first_ammo_bar->free();
        return;
    }
    first_ammo_bar->setRect(107, 59);
    first_ammo_bar->setRectWH(12, 20);
    bulletBar.push_back(first_ammo_bar);

    baseObject* mid_ammo_bar = new baseObject[8];
    for(int i=0; i<8; i++){
        baseObject* mid_bar = (mid_ammo_bar + i);
        mid_bar->loadIMG("img//Bar//mid_ammo.png", screen);
        bool mRet = mid_bar->loadIMG("img//Bar//mid_ammo.png", screen);
        if(!mRet) {
            mid_bar->free();
            return;
        }
        mid_bar->setRect((107+12+12*i), 59);
        mid_bar->setRectWH(12, 20);
        bulletBar.push_back(mid_bar);
    }
    
    baseObject* end_ammo_bar = new baseObject[1];
    end_ammo_bar->loadIMG("img//Bar//end_ammo.png", screen);
    bool beRet = end_ammo_bar->loadIMG("img//Bar//end_ammo.png", screen);
    if(!eRet) {
        end_ammo_bar->free();
        return;
    }
    end_ammo_bar->setRect((107+108), 59);
    end_ammo_bar->setRectWH(12, 20);
    bulletBar.push_back(end_ammo_bar);


}

void mainProtagonist::healthManagement(SDL_Renderer* screen){
    baseObject healthMenu;
    healthMenu.loadIMG("img//statusBarEmpty.png", screen);
    bool hRet = healthMenu.loadIMG("img//statusBarEmpty.png", screen);
    if(!hRet) {
        healthMenu.free();
        return;
    }
    healthMenu.setRect(0, 0);
    healthMenu.setRectWH(149*2, 64*2);
    healthMenu.render(screen, NULL);

    for(int i=0; i<health/10; i++){
        healthBar[i]->render(screen, NULL);
    }
    for(int i=0; i<(ammo/5 + (ammo%5 > 0? 1 : 0)); i++){
        bulletBar[i]->render(screen, NULL);
    }
}

void mainProtagonist::doPlayer(map& map_data){
    x_val = 0;
    y_val = 0;

    if(input_type.left ==1){
        x_val -= player_speed;
    } else if (input_type.right ==1){
        x_val += player_speed;
    }
    if (input_type.up ==1){
        y_val -= player_speed;
    } else if (input_type.down ==1){
        y_val += player_speed;
    }
    if(abs(x_val) + abs(y_val) >= 1.4*player_speed){
        x_val = x_val *3 /4;
        y_val = y_val *3 /4;
    }

    checkCollision(map_data);
    centerEntityOnMap(map_data);
}

void mainProtagonist::centerEntityOnMap(map& map_data){
    if(map_data.start_x>=0 && map_data.start_x<(map_data.max_x-screen_width)
    && map_data.start_y>=0 && map_data.start_y<(map_data.max_y-screen_height)){
        x_screen_pos = screen_width/2 + width_frame/2;
        y_screen_pos = screen_height/2 + height_frame/2;
    }

    map_data.start_x = x_pos - (screen_width/2);
    if(map_data.start_x < 0){
        map_data.start_x = 0;
        x_screen_pos = x_pos + width_frame/2;
    } else if(map_data.start_x + screen_width >= map_data.max_x){
        map_data.start_x = map_data.max_x - screen_width;
        int tempx=x_pos/1.0;
        x_screen_pos = tempx%1280 + width_frame/2;//supposed to be screen width
    }

    map_data.start_y = y_pos - (screen_height/2);
    if(map_data.start_y < 0){
        map_data.start_y = 0;
        y_screen_pos = y_pos + height_frame/2;
    } else if(map_data.start_y + screen_height >= map_data.max_y){
        map_data.start_y = map_data.max_y - screen_height;
        int tempy=y_pos/1.0;
        y_screen_pos = tempy%768 + height_frame/2;//supposed to be screen height
    }
}

// #include <iostream>
void mainProtagonist::checkCollision(map& map_data){
    int x1 = 0;
    int x2 = 0;
    
    int y1 = 0;
    int y2 = 0;

    //Check horizontal
    int height_min = height_frame < tile_size ? height_frame : tile_size;

    x1 = (x_pos + 5 + x_val)/tile_size;
    x2 = (x_pos - 5 + x_val + width_frame - 1)/tile_size;

    y1 = (y_pos + 5)/tile_size;
    y2 = (y_pos - 5 + height_min - 1)/tile_size;

    // std::cout << x1 << " " << x2 << std::endl << y1 << " " << y2 << std::endl;

    if(x1>=0 && x2<max_level_map_x && y1>=0 && y2<max_level_map_y){
        if(x_val > 0){
            //mainProtagonist is moving to the right
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y1][x2])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y2][x2]) ){
                x_pos = x2*tile_size;
                x_pos -= (width_frame);
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
    x1 = (x_pos + 5)/tile_size;
    x2 = (x_pos - 5 + width_min)/tile_size;

    y1 = (y_pos + 5 + y_val)/tile_size;
    y2 = (y_pos - 5 + y_val + height_frame - 1)/tile_size;

    if(x1>=0 && x2<max_level_map_x && y1>=0 && y2<max_level_map_y){
        if(y_val > 0){
            //is going down
            if(SDLCommonFunc::checkTileCollision(map_data.tile[y2][x1])
            || SDLCommonFunc::checkTileCollision(map_data.tile[y2][x2]) ){
                y_pos = y2*tile_size;
                y_pos -= (height_frame);
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

SDL_Rect mainProtagonist::getRectFrame(){
    SDL_Rect tmp_rect;
    tmp_rect.x = rect.x;
    tmp_rect.y = rect.y;
    tmp_rect.w = width_frame;
    tmp_rect.h = height_frame;
    return tmp_rect;
}

void mainProtagonist::handleBullet(SDL_Renderer* des, int xm, int ym){
    for(int i =0; i<bullet_list.size(); i++){
        projectile* p_bullet = bullet_list.at(i);
        if(p_bullet != NULL){
            if(p_bullet->get_is_move() == true){
                if(p_bullet->get_bullet_type() == projectile::MAIN_ARROW){
                    float angle = SDLCommonFunc::calAngle(xm, ym, x_screen_pos, y_screen_pos);
                    p_bullet->handleMove(screen_width, screen_height);
                    p_bullet->renderArrow(des, angle);
                } /*else {
                    p_bullet->handleMove(screen_width, screen_height);
                    p_bullet->render(des);
                }*/
            } else {
                bullet_list.erase(bullet_list.begin() + i);
                if(p_bullet != NULL){
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        } 
    }
}

void mainProtagonist::removeBullet(const int& idx){
    int size = bullet_list.size();
    if(size > 0 && idx < size){
        projectile* p_bullet = bullet_list.at(idx);
        bullet_list.erase(bullet_list.begin() + idx);
        if(p_bullet){
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}