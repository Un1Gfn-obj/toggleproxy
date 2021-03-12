#ifndef C_H
#define C_H

typedef enum {
  UNKNOWN=0,
  DISCONNECTED=1,
  CONNECTED_OFF=2,
  CONNECTED_ON=3
} Status;

extern Status proxy_status();

extern void check_iface();

extern void on2off();
extern void off2on();

extern void cf_force_refresh();

#endif
