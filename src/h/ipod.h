#ifndef MYHEADER_H

#define MYHEADER_H


#include <display.h>
#include <bluetooth.h>
#include <const.h>


void init_ipod(void);
void read_ipod(void);

void poll_ipod(void);


void kickStart(void);
void sendToPod(byte , byte , byte [], byte );
void sendToPod(byte , byte [], byte [], byte );

byte getCheckSum(byte );
void sendGetModeToPod(void);
void reset(void);

byte playlist_pos[size_int_32];
byte playlist_total[size_int_32];


#endif /* MYHEADER_H */
