#include <iostream>
#include <assert.h>
#include "helper.h"

#include "Player.h"

/*
 * Should receive and update the team hill status messages
 */
void test_player_payload_type2();
/*
 * Should receive and update the global status messages
 */
void test_player_payload_type3();
/*
 * Should receive the global max points and set gameStatus to Start
 */
void test_player_payload_type4();
/*
 * Should send a ping if in play Status
 */
void test_player_ping();
/*
 * Should end the game if max points reached
 */
void test_player_game_end();

void test_player()
{
    PACKAGE("Player Class")

    test_player_payload_type2();
    test_player_payload_type3();
    test_player_payload_type4();

    test_player_ping();

    test_player_game_end();

    PRINT("Successfully tested.")
}

void test_player_game_end()
{
    Player p; 

    p.gameStatus = PLAY;
    p.global_points_max = 50;

    p.team0_global_status = 20;
    p.team1_global_status = 20;
    p.update();

    assert(p.gameStatus == PLAY);

    p.team0_global_status = 100;
    p.update();

    assert(p.gameStatus == END);

    p.gameStatus = PLAY;
    p.team0_global_status = 10;
    p.team1_global_status = 200;
    p.update();

    assert(p.gameStatus == END);
}

void test_player_ping()
{
    Player p; 

    assert(p.gameStatus == INIT);
    assert(p.teamId == -1);

    p.teamId = 1;

    p.ping(100);
    assert(called_send_radio() == 0);

    p.gameStatus = PLAY;
    p.ping(100);

    assert(p.teamId == 1);
    assert(called_send_radio() == 1);
    assert(static_payload.type == 0);
    assert(((int)static_payload.message) == 1);
    assert(static_payload.message2 == 0);
    assert(p.last_success_ping == 100);
}

void test_player_payload_type2()
{
    Player p; 

    payload py;  
    py.type = 2;
    py.message = 10;
    py.message2 = 100;

    p.read_payload(py);

    assert(p.team0_hill_status == 10);
    assert(p.team1_hill_status == 100);
}

void test_player_payload_type3()
{
    Player p; 

    payload py;  
    py.type = 3;
    py.message = 10;
    py.message2 = 100;

    p.read_payload(py);

    assert(p.team0_global_status == 10);
    assert(p.team1_global_status == 100);
}

void test_player_payload_type4()
{
    Player p; 

    payload py;  
    py.type = 4;
    py.message = LOWBYTE((int)155);
    py.message2 = HIGHBYTE((int)155);

    p.gameStatus = PLAY;

    p.read_payload(py);

    assert(p.team0_global_status == 0);
    assert(p.team1_global_status == 0);

    p.gameStatus = INIT;

    p.read_payload(py);

    assert(p.global_points_max == 155);
    assert(p.gameStatus == START);
}

