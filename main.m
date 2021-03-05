// /opt/theos/sdks/iPhoneOS14.4.sdk/**/*.h

#include <assert.h>

#import <Foundation/Foundation.h>
// #import <NetworkExtension/NEProxySettings.h>

#define COUNT 9

// Use "Global Proxy Settings Constants" instead?
// https://developer.apple.com/documentation/cfnetwork/global_proxy_settings_constants?language=objc
static CFDictionaryRef checkInt_dict=NULL;
#define checkInt(K,V) checkInt0(CFSTR(K),V)
static void checkInt0(const void *key,const int64_t value){
  assert(CFDictionaryContainsKey(checkInt_dict,key));
  CFTypeRef r=CFDictionaryGetValue(checkInt_dict,key); // CFShow(r);
  assert(CFGetTypeID(r)==CFNumberGetTypeID());
  CFNumberRef n=r;
  int64_t v=-1;
  assert(CFNumberGetType(n)==kCFNumberSInt64Type&&
         CFNumberGetByteSize(n)==64/8&&
         CFNumberGetValue(n,kCFNumberSInt64Type,&v)&&
         v==value);
}

static void cfd(){

  // https://developer.apple.com/documentation/corefoundation?language=objc
  // https://developer.apple.com/documentation/corefoundation/cfdictionary?language=objc
  CFDictionaryRef d=CFNetworkCopySystemProxySettings();
  assert(d);
  assert(CFDictionaryGetCount(d)==COUNT);

  checkInt_dict=d;
  /* [0] */ checkInt("HTTPEnable",+1); // HTTPEnable CFNumber +1
  /* [1] */ checkInt("HTTPPort",+8080);
  /* [2] */ // checkInt("",);
  /* [3] */ // checkInt("",);
  /* [4] */ checkInt("FTPPassive",+1);
  /* [5] */ checkInt("HTTPSPort",+8080);
  /* [6] */ // checkInt("",);
  /* [7] */ checkInt("HTTPSEnable",+1);
  /* [8] */ // checkInt("",);
  checkInt_dict=NULL;
  fprintf(stderr,"\n");

  const void *keys[COUNT],*values[COUNT];
  bzero(keys,COUNT*sizeof(void*));
  bzero(values,COUNT*sizeof(void*));
  CFDictionaryGetKeysAndValues(d,keys,values);
  // CFDictionaryApplyFunction

  // for( const int *p=(int[]){0,1,2,3,4,5,6,7,8,-777}; *p>=0; ++p ){
  for( const int *p=(int[]){2,3,6,8,-777}; *p>=0; ++p ){
    assert(keys[*p]&&values[*p]);
    fprintf(stderr,"[%d] ",*p);CFShow(keys[*p]);
    CFShow(values[*p]);
    fprintf(stderr,"\n");
  }

  CFRelease(d);d=NULL;

}

/*static void nsd(){@autoreleasepool{
  NSDictionary *nsd=(__bridge NSDictionary*)CFNetworkCopySystemProxySettings();
  NSDictionary *nsd=CFBridgingRelease(CFNetworkCopySystemProxySettings());
  // https://developer.apple.com/documentation/networkextension/neproxysettings?language=occ
  NEProxySettings *s=[NEProxySettings new];
  printf("%d\n",s.HTTPEnabled);
  printf("%d\n",proxySettings.HTTPEnabled);
}}*/

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
