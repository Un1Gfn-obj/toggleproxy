// /opt/theos/sdks/iPhoneOS14.4.sdk/**/*.h

#include <assert.h>

#import <Foundation/Foundation.h>
// #import <NetworkExtension/NEProxySettings.h>

#define COUNT 9

/*static void nsd(){@autoreleasepool{
  NSDictionary *nsd=(__bridge NSDictionary*)CFNetworkCopySystemProxySettings();
  NSDictionary *nsd=CFBridgingRelease(CFNetworkCopySystemProxySettings());
  // https://developer.apple.com/documentation/networkextension/neproxysettings?language=occ
  NEProxySettings *s=[NEProxySettings new];
  printf("%d\n",s.HTTPEnabled);
  printf("%d\n",proxySettings.HTTPEnabled);
}}*/

// https://developer.apple.com/documentation/corefoundation?language=objc
// https://developer.apple.com/documentation/corefoundation/cfdictionary?language=objc
static void cfd(){

  CFDictionaryRef d=CFNetworkCopySystemProxySettings();
  assert(d);
  assert(CFDictionaryGetCount(d)==COUNT);

  const void *keys[COUNT],*values[COUNT];
  bzero(keys,COUNT*sizeof(void*));
  bzero(values,COUNT*sizeof(void*));
  CFDictionaryGetKeysAndValues(d,keys,values);
  // CFDictionaryApplyFunction

  // assert(!keys[COUNT]);
  // assert(!values[COUNT]);
  // assert(keys[COUNT-1]);
  // assert(values[COUNT-1]);

  // HTTPEnable CFNumber +1
  assert(CFDictionaryContainsKey(d,CFSTR("HTTPEnable")));
  CFTypeRef r=CFDictionaryGetValue(d,CFSTR("HTTPEnable")); // CFShow(r);
  assert(CFGetTypeID(r)==CFNumberGetTypeID());
  CFNumberRef n=r;
  int64_t v=-1;
  assert(CFNumberGetType(n)==kCFNumberSInt64Type&&
         CFNumberGetByteSize(n)==64/8&&
         CFNumberGetValue(n,kCFNumberSInt64Type,&v)&&
         v==1);

  fprintf(stderr,"\n");
  for(int i=1;i<COUNT;++i){
    assert(keys[i]&&values[i]);
    CFShow(keys[i]);
    CFShow(values[i]);
    fprintf(stderr,"\n");
  }

  CFRelease(d);d=NULL;

}

// int main(int argc,char *argv[],char *envp[]){@autoreleasepool{
int main(){

  puts("");
  puts( "bld@" /*__DATE__*/ __TIME__ );
  time_t rawtime=0;
  time(&rawtime);
  struct tm *timeptr=localtime(&rawtime);
  assert(timeptr);
  // puts(asctime(timeptr));
  printf("cur@%.2d:%.2d:%.2d\n",
         timeptr->tm_hour,
         timeptr->tm_min,
         timeptr->tm_sec);

  cfd();

  puts("");
  return 0;

}
