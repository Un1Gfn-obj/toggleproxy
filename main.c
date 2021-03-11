// /opt/theos/sdks/iPhoneOS14.4.sdk/**/*.h

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./status.h"

// /Library/Preferences: symbolic link to ../private/var/preferences
// /Library/Preferences/SystemConfiguration/preferences.plist
// /private/var/preferences/SystemConfiguration/preferences.plist
#define eprintf(...) fprintf(stderr,__VA_ARGS__)

Status proxy_status();
void check_iface();
void on2off();
// void off2on();

static void buildinfo(){
  eprintf( "bld@" /*__DATE__*/ __TIME__ "\n" );
  time_t rawtime=0;
  time(&rawtime);
  struct tm *timeptr=localtime(&rawtime);
  assert(timeptr);
  // puts(asctime(timeptr));
  eprintf("cur@%.2d:%.2d:%.2d\n",
          timeptr->tm_hour,
          timeptr->tm_min,
          timeptr->tm_sec);
}

static void assertion_not_disabled(){
  int t=8;
  assert((t=t*t));
  if(t!=64){
    eprintf("assertion disabled - side effects not applied - abort\n");
    abort();
  }
}

// int main(int argc,char *argv[],char *envp[]){
int main(){

  // eprintf("\n");

  assertion_not_disabled();
  buildinfo();
  eprintf("\n");

  check_iface();

  const Status s=proxy_status();
  switch(s){

  case DISCONNECTED:
    eprintf("disconnected\n");
    return 0;
    break;

  case CONNECTED_OFF:
    eprintf("proxy is off\n");
    return 0;
    break;

  case CONNECTED_ON :
    eprintf("proxy is on\n");
    on2off();
    // eprintf("turning off proxy ...\n");
    // assert(proxy_status()==CONNECTED_OFF);
    // eprintf("... done\n");
    return 0;
    break;

  default:
    assert(false);
    break;
  }

  eprintf("\n");
  return 0;

}
