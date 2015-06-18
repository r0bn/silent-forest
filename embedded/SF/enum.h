/*
 * Global used Enums
 */


#ifndef __ENUM_H__
#define __ENUM_H__

typedef unsigned char byte;

// Describes the Teams
// TODO: Make Upper case letter
enum Team
{
    Neutral,
    Red,
    Blue
};

// Describes the game states
enum GameStatus
{
    INIT,
    START,
    PLAY,
    END
};

#endif
