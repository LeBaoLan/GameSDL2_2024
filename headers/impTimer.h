#pragma once

class impTimer{
    public:
        impTimer();
        ~impTimer();

        void start();
        void stop();
        void paused();
        void unpaused();

        int get_ticks();
        bool check_is_started();
        bool check_is_paused();
    private:
        int start_tick;
        int paused_tick;
        bool is_paused;
        bool is_started;
};