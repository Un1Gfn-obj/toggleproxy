// /opt/theos/sdks/iPhoneOS14.4.sdk/**/*.h

#include <assert.h>
#include <stdio.h>
#include <time.h>

#define eprintf(...) fprintf(stderr,__VA_ARGS__)

void cfd();

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

// int main(int argc,char *argv[],char *envp[]){@autoreleasepool{
int main(){

  eprintf("\n");
  buildinfo();
  eprintf("\n");

  cfd();
  eprintf("Proxy is on. Exit normally.\n");

  eprintf("\n");
  return 0;

}
