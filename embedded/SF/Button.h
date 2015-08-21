#ifndef __BUTTON_H__
#define __BUTTON_H__

enum button_status{
    DOWN,
    PRESSED,
    RELEASED,
    CLEAR
};

class Button
{
    private:
        unsigned int debounce_time = 50;
        unsigned int pressed_action_timer;
        unsigned long pressed_start_time;
        unsigned int pressed_time;
        unsigned long last_change;
        bool single_shot;
        button_status last_state = CLEAR;


    protected:

    public:
        byte pin;
        button_status state = CLEAR;
        Button();
        Button(byte pin, unsigned int snd_action_time, bool single_shot);
        void setup();
        void update();

};

#endif
