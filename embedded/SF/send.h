/*
 * This functions has the purpose to abstract the logic of the send out
 * functions.
 * In production the functions in the main.cpp are used
 * In testing the functions in the test folder are used
 *
 * This makes it possible to use the send in the class's and still able 
 * to test on the pc
 */

#ifndef __SEND_H__
#define __SEND_H__

typedef unsigned char byte;

/*
 * Message Types:
 * 0 : Player Ping
 * 1 : Hill Ping
 * 2 : Team Hill Status for Player
 * 3 : Team Global Status for Player
 * 4 : Ini Global Status Message and start
 * 5 : ID Handshake
 */
struct payload
{
  byte type;
  byte message;
  byte message2;
};
// static payload
extern payload static_payload;

// send with xbee
void send_xbee(payload p);
// send wit radio
int send_radio(payload p);

// Build's a payload on a static variable for standard and performance reasons
payload build_payload(byte type, byte message, byte message2);

#endif
