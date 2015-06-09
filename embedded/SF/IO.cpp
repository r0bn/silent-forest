/*IO CLASS with buttons
setStatus1()
setStatus2()
setTeamRed()
setTeamBlue()
*/
#include "Arduino.h"
#include "IO.h"


void IO::reset()
{
    digitalWrite(status_led_1_pin, LOW);
    digitalWrite(status_led_2_pin, LOW);

    for(byte i=0; i<sizeof(red_leds); i++ )
    {
        digitalWrite(red_leds[i], LOW);
    }

    for(byte i=0; i<sizeof(blue_leds); i++ )
    {
        digitalWrite(blue_leds[i], LOW);
    }
}

void IO::setGlobalTeam(byte blue, byte red)
{
    team_blue_global_status = blue;
    team_red_global_status = red;
}

void IO::setHillTeam(byte blue, byte red)
{
    team_blue_hill_status = blue;
    team_red_hill_status = red;
}

void IO::setModeBlue(led_mode mode)
{
    blue_mode = mode;
}

void IO::setModeRed(led_mode mode)
{
    red_mode = mode;
}

void IO::setStatus1(led_mode mode)
{
    status_led_1_mode = mode;
}

void IO::setStatus2(led_mode mode)
{
    status_led_2_mode = mode;
}

void IO::setModeTeam(led_mode mode)
{
    blue_mode = mode;
    red_mode = mode;
}

void IO::switchTo(team team_to_switch)
{
    show_team = team_to_switch;
}

void IO::setup()
{
    if(role == HILL)
    {
        status_led_1_pin = 8;
        status_led_2_pin = 9;
        
        button_2_pin = A4;
    }

    pinMode(status_led_1_pin, OUTPUT);
    digitalWrite(status_led_1_pin, LOW);
    pinMode(status_led_2_pin, OUTPUT);
    digitalWrite(status_led_2_pin, LOW);

    for(byte i=0; i<sizeof(red_leds); i++ )
    {
        pinMode(red_leds[i], OUTPUT);
        digitalWrite(red_leds[i], LOW);
    }

    for(byte i=0; i<sizeof(blue_leds); i++ )
    {
        pinMode(blue_leds[i], OUTPUT);
        digitalWrite(blue_leds[i], LOW);
    }

    pinMode(button_1_pin, INPUT);
    digitalWrite(button_1_pin, HIGH);
    pinMode(button_2_pin, INPUT);
    digitalWrite(button_2_pin, HIGH);
}

void IO::update()
{
    updateInputs();
    updateLEDs();
}

void IO::updateInputs()
{

}

void IO::updateLEDs()
{
    updateLED(status_led_1_pin, status_led_1_mode);
    updateLED(status_led_2_pin, status_led_2_mode);

    if(role == PLAYER)
    {
        byte red = 0;
        byte blue = 0;

        if(show_team == TEAM_HILL)
        {
            blue = team_blue_hill_status;
            red = team_red_hill_status;
        }
        else if(show_team == TEAM_GLOBAL)
        {
            blue = team_blue_global_status;
            red = team_red_global_status;
        }
        

        for(byte i=0; i<sizeof(blue_leds); i++)
        {
            if(i < blue)
            {
                updateLED(blue_leds[i], blue_mode);
            }
            else
            {
                updateLED(blue_leds[i], OFF);
            }
        }

        for(byte i=0; i<sizeof(red_leds); i++)
        {
            if(i < red)
            {
                updateLED(red_leds[i], red_mode);
            }
            else
            {
                updateLED(red_leds[i], OFF);
            }
        }
    }
}

void IO::updateLED(byte pin, led_mode pin_mode)
{
    if((millis() - pulse_timer) > pulse_time)
    {
        toggle_leds = !toggle_leds;
        pulse_timer = millis();
    }

    if(pin_mode == ON || (pin_mode == PULSE && toggle_leds))
    {
        digitalWrite(pin, HIGH);
    }
    else
    {
        digitalWrite(pin, LOW);
    }
}