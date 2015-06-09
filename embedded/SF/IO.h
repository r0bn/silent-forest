#ifndef __IO_H__
#define __IO_H__

enum led_mode
{
    OFF,
    ON,
    PULSE
};

enum module_role
{
    PLAYER,
    HILL
};

enum team
{
    TEAM_HILL,
    TEAM_GLOBAL
};

class IO
{
    private:
        led_mode status_led_1_mode;
        led_mode status_led_2_mode;
        led_mode blue_mode;
        led_mode red_mode;
        unsigned int pulse_time = 500;
        unsigned long pulse_timer = 0;
        bool toggle_leds;

        byte blue_leds[5] = {A4, A3, A2, A1, A0};
        byte red_leds[5] = {2, 3, 4, 5, 6};
        byte status_led_1_pin = 7;
        byte status_led_2_pin = 8;
        
        byte button_1_pin = A5;
        byte button_2_pin;

    protected:


    public:
        byte team_blue_hill_status;
        byte team_blue_global_status;
        byte team_red_hill_status;
        byte team_red_global_status;

        module_role role = PLAYER;

        team show_team = TEAM_HILL;

        void reset();
        void setGlobalTeam(byte blue, byte red);
        void setHillTeam(byte blue, byte red);
        void setModeBlue(led_mode mode);
        void setModeRed(led_mode mode);
        void setModeTeam(led_mode mode);
        void setup();
        void setStatus1(led_mode mode);
        void setStatus2(led_mode mode);
        void switchTo(team team_to_switch);
        void update();
        void updateInputs();
        void updateLED(byte pin, led_mode pin_mode);
        void updateLEDs();

};
#endif
