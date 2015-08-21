#include "Arduino.h"
#include "Button.h"

Button::Button()
{

}

Button::Button(byte pin, unsigned int snd_action_time, bool single_shot)
{
    this->pin = pin;
    this->single_shot = single_shot;
    pressed_action_timer = snd_action_time;

    setup();
}

void Button::update()
{
    if(single_shot && state != CLEAR)
        state = CLEAR;
    
    if(millis() - last_change > debounce_time)
    {
        if(!digitalRead(pin) && (last_state == CLEAR || last_state == RELEASED))
        {
            state = DOWN;
            pressed_start_time = millis();
            last_change = millis();
        }
        else if(!digitalRead(pin) && last_state == DOWN && millis() - pressed_start_time > pressed_action_timer)
        {
            state = PRESSED;
            last_change = millis();
        }

        if(digitalRead(pin) && (last_state == DOWN || last_state == PRESSED))
        {
            state = RELEASED;
            last_change = millis();
        }
        else if(digitalRead(pin) && last_state == RELEASED)
        {
            state = CLEAR;
            last_state = state;
            last_change = millis();
        }
        if(state != CLEAR)
            last_state = state;
        
    }
}

void Button::setup()
{
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
}
