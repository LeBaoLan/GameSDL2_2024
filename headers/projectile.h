#pragma once

#include "baseObject.h"
#include "commonFunc.h"

class projectile : public baseObject{
    public:
        projectile();
        ~projectile();

        enum bulletDir{
            DIR_RIGHT = 20,
            DIR_LEFT = 21,
            MAIN_MULTI_DIR = 22,
        };

        enum bulletType{
            TMP_MAIN_BULLET = 50,
            FROG_SALIVA = 51,
            MAIN_ARROW = 52,
        };

        void set_x_val(float xVal){p_x_val = xVal;}
        void set_y_val(float yVal){p_y_val = yVal;}
        int get_x_val() const {return p_x_val;}
        int get_y_val() const {return p_y_val;}

        void set_is_move(const bool& isMove){is_move = isMove;}
        bool get_is_move() const {return is_move;}

        void set_bullet_dir(const unsigned int& bulletDir){bullet_dir = bulletDir;}
        unsigned int get_bullet_dir() const {return bullet_dir;}

        void set_bullet_type(const unsigned int& bulletType) {bullet_type = bulletType;}
        unsigned int get_bullet_type() const {return bullet_type;}

        void handleMove(const int& x_border, const int& y_border);
        bool loadImgBullet(SDL_Renderer* des);
        
        void renderArrow(SDL_Renderer* des, float angle);
    private:
        float p_x_val;
        float p_y_val;
        bool is_move;
        unsigned int bullet_dir;
        unsigned int bullet_type;
};