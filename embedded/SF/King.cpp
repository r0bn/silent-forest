#include "King.h"

void King::hill_log(Team team)
{
    King::hill_logs.push((int) team);
}

void King::update()
{
    // Count contacts
    King::hill_logs.count_elements();

    // Top occupants hills from team this round 
    Team current_occupant = (Team) King::hill_logs.top_occurence();

    King::global_log_teams[(int)current_occupant]++;
}
