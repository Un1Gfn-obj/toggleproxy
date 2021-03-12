// https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
#if defined __OBJC__ || defined __cplusplus
#error
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./c.h"
#include "./privilege.h"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"
#define eprintf(...) fprintf(stderr,__VA_ARGS__)

// m.m
// extern void ns_force_refresh();

static void assertion_not_disabled(){
  int t=8;
  assert((t=t*t));
  if(t!=64){
    eprintf("assertion disabled - side effects not applied - abort\n");
    abort();
  }
}

static void ver(){
  eprintf( "bld@" __TIME__ " - " __DATE__ "\n" );
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

static void hint_manual_op(){
  eprintf("To make changes take effect:\n");
  // https://support.apple.com/lt-lt/guide/mdm/mdm90f60c1ce/web
  eprintf("(1) Disable WLAN in com.apple.Preferences \n");
  // https://developer.apple.com/design/human-interface-guidelines/ios/bars/status-bars/
  eprintf("(2) Wait until Wi-Fi icon disappears from status bar\n");
  eprintf("(3) Enable WLAN in com.apple.Preferences\n");
  eprintf("(4) Wait until Wi-Fi icon reappears on status bar\n");
}

// https://github.com/Un1Gfn-electronics/df/search?q=unused
// https://clang.llvm.org/docs/AttributeReference.html#maybe-unused-unused
int main(int argc,char *argv[],__attribute__((unused))char *envp[]){

  assertion_not_disabled();
  privilege_drop();

  static const char *const arg="ver";
  if(argc==2){
    assert(0==strcmp(arg,argv[1]));
    ver();
    exit(0);
  }
  assert(argc==1);
  eprintf("Run \"%s %s\" for build info\n",argv[0],arg);

  check_iface();
  switch(proxy_status()){

  case DISCONNECTED:
    eprintf("Disconnected\n");
    break;

  case CONNECTED_OFF:
    eprintf("Proxy is off\n");
    eprintf(GREEN"Turning on proxy ..."RESET"\n");
    off2on();
    // cf_force_refresh();
    hint_manual_op();
    // assert(proxy_status()==CONNECTED_ON);
    // eprintf("proxy is turned off\n");
    break;

  case CONNECTED_ON :
    eprintf("Proxy is on\n");
    eprintf(RED"Turning off proxy ..."RESET"\n");
    on2off();
    // cf_force_refresh();
    hint_manual_op();
    // assert(proxy_status()==CONNECTED_OFF);
    // eprintf("proxy is turned off\n");
    break;

  default:
    assert(false);
    break;

  }

  return 0;

}
