#include <iostream>
#include <assert.h>
#include "helper.h"

#include "send.h"

payload static_payload;

void send_xbee(payload p)
{
    return;
}


int send_radio(payload p)
{
    c_send_radio = 1;
    return 1;
}

payload build_payload(byte type, byte message, byte message2)
{
    static_payload.type = type;
    static_payload.message = message;
    static_payload.message2 = message2;

    return static_payload;
}

int called_send_radio()
{
    int erg = c_send_radio;
    c_send_radio = 0;
    return erg;
}
