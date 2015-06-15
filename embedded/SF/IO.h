#ifndef __IO_H__
#define __IO_H__

#include "Button.h"
/*
 * Different modes a LED can performe
 */
enum led_mode
{
    OFF,
    ON,
    PULSE
};

/*
 * Different roles
 */
enum module_role
{
    PLAYER,
    HILL
};

/*
 * Different status that can be shown on team leds
 */
enum team
{
    TEAM_HILL,
    TEAM_GLOBAL
};

enum eepAdress
{
    EEP_MAX_HILL_POINTS = 100
};

class IO
{
    private:
        /*
         * modes of the differen LED groups (2x status, 2x team)
         */
        led_mode status_led_1_mode;
        led_mode status_led_2_mode;
        led_mode blue_mode;
        led_mode red_mode;
        /*
         * periode LEDs are flashing
         */
        unsigned int pulse_time = 500;
        /*
         * timer for the flashing LEDs
         */
        unsigned long pulse_timer = 0;
        /*
         * defines if LEDs are on or off in PULSE mode
         */
        bool toggle_leds;
        /*
         * defines which status is shown on the team LEDs
         */
        team show_team = TEAM_HILL;

        /*
         * pins of the led and status LEDs
         */
        byte blue_leds[5] = {A4, A3, A2, A1, A0};
        byte red_leds[5] = {2, 3, 4, 5, 6};
        byte status_led_1_pin;
        byte status_led_2_pin;

        /*
         * pins of the button(s)
         */
        byte button_1_pin = A5;
        byte button_2_pin;

    protected:


    public:
        /*
         * saves the global/hill status
         */
        Button buttons[2];
        byte team_blue_hill_status;
        byte team_blue_global_status;
        byte team_red_hill_status;
        byte team_red_global_status;

        /*
         * role of the module
         */
        module_role role = PLAYER;
        
        /*
         * constructor
         */
        IO(module_role role);
        /*
         * switching off all LEDs
         */

        void eepWrite(int adress, byte data);
        void eepWrite(int adress, int data);
        void eepWrite(int adress, long data);
        void eepWrite(int adress, float data);
        
        void eepRead(int adress, byte data);
        void eepRead(int adress, int data);
        void eepRead(int adress, long data);
        void eepRead(int adress, float data);

        /*
         * Initialize status leds
         */
        void reset();
        /*
         * set the global status for both teams
         */
        void setGlobalTeam(byte blue, byte red);
        /*
         * set the hill status for both teams
         */
        void setHillTeam(byte blue, byte red);
        /*
         * set the mode how the blue, red or both leds will shine (OFF, ON, PUlSE
         */
        void setModeBlue(led_mode mode);
        void setModeRed(led_mode mode);
        void setModeTeam(led_mode mode);
        /*
         * initialize the LEDs (OFF) and buttons
         */
        void setup();
        /*
         * set the mode how the status LEDs will shine (OFF, ON, PULSE)
         */
        void setStatus1(led_mode mode);
        void setStatus2(led_mode mode);
        /*
         * set which status (TEAM_HILL, TEAM_GLOBAL) the team LEDs should show
         */
        void switchTo(team team_to_switch);
        /*
         * update the in- and outputs 
         */
        void update();
        /*
         * update the inputs
         */
        void updateInputs();
        /*
         * update a single LED according to pin_mode (OFF, ON, PULSE)
         */
        void updateLED(byte pin, led_mode pin_mode);
        /*
         * update all LEDs
         */
        void updateLEDs();

};
#endif
