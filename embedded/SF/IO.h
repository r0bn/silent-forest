#ifndef __IO_H__
#define __IO_H__

class IO
{
    private:

#if PLAYER
        byte blue_leds[] = {A0, A1, A2, A3, A4};
        byte red_leds[] = {2, 3, 4, 5, 6};
        byte status_led_1 = 7;
        byte status_led_2 = 8;
        
        byte buttons[] = {A5};
#endif

#if HILL
        byte status_led_1 = A0;
        byte status_led_2 = A1;
        
        byte buttons[] = {A4, A5};
#endif


    protected:


    public:
        void setup();
        void update();
}
