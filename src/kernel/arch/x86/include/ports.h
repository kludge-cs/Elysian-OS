#ifndef __PORTS_H
#define __PORTS_H

void port_out(unsigned short port, unsigned char data);
unsigned char port_in(unsigned short port);

#endif