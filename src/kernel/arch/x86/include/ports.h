#ifndef __PORTS_H
#define __PORTS_H

extern void port_out(unsigned short port, unsigned char data);
extern unsigned char port_in(unsigned short port);

#endif