#include <netinet/in.h> // INET6_ADDRSTRLEN
#include "./secret.h"

#import <Foundation/Foundation.h>
// #import <NetworkExtension/NEProxySettings.h>

#define COUNT 9
#define eprintf(...) fprintf(stderr,__VA_ARGS__)

/*In case static_assert fails:
(1) On 32-bit iOS with 32-bit pointers and kCFNumberSInt32Type dictionary values, generic int32_t parameter may work
(2) If 32/64-bit things are mixed, simply cast everything to 64-bit
(3) If a generic parameter is not feasible, do the following
static void check(...const void *data...){
  ...
  }else if(typeid==CFNumberGetTypeID()){
    ...
    assert(...v==*((int64_t*)data))
    ...
  }else if(typeid==CFStringGetTypeID()){
    ...
    assert(...0==strcmp((const char*)data,buffer))
    ...
  }
  ...
}
void cfd(){
  ...
  check("HTTPEnable",&(int64_t){+1},CFNumberGetTypeID());
  check("HTTPSProxy",PROXY_IP,CFStringGetTypeID());
  ...
}*/
static_assert(sizeof(int64_t)==sizeof(char*),"");

// Use "Global Proxy Settings Constants" instead?
// https://developer.apple.com/documentation/cfnetwork/global_proxy_settings_constants?language=objc
static CFDictionaryRef checkInt_dict=NULL;
// https://www.ismp.org/resources/misidentification-alphanumeric-symbols
#define check1nt(K,V) check(CFSTR(K),          V ,CFNumberGetTypeID())
#define check8tr(K,V) check(CFSTR(K),(int64_t)(V),CFStringGetTypeID())
static void check(const void *key,const int64_t value,CFTypeID typeid){
  assert(CFDictionaryContainsKey(checkInt_dict,key));
  const CFTypeRef r=CFDictionaryGetValue(checkInt_dict,key);assert(r); // CFShow(r);
  assert(CFGetTypeID(r)==typeid);
  if(false){
    ;
  }else if(typeid==CFNumberGetTypeID()){
    const CFNumberRef n=r;
    int64_t v=-1;
    assert(CFNumberGetType(n)==kCFNumberSInt64Type&&
           CFNumberGetByteSize(n)==64/8&&
           CFNumberGetValue(n,kCFNumberSInt64Type,&v)&&
           v==value);
  }else if(typeid==CFStringGetTypeID()){
    const char *const value=(char*)value; // -Wshadow
    // const unsigned long sz=strlen(value)+1;
    const CFStringRef s=r;
    assert(strlen(PROXY_IP)==CFStringGetLength(s));
    char buffer[INET6_ADDRSTRLEN];
    bzero(buffer,INET6_ADDRSTRLEN*sizeof(char));
    assert(CFStringGetCString(s,buffer,INET6_ADDRSTRLEN,kCFStringEncodingASCII));
    assert(!buffer[INET6_ADDRSTRLEN-1]);
    assert(0==strcmp(PROXY_IP,buffer));
    // assert(CFStringGetCString(s,buffer,sz,kCFStringEncodingASCII)&&
    //        !buffer[sz-1]&&
    //        0==strcmp(PROXY_IP,buffer));
  }else{
    assert(false);
  }
}

// static void check(const void *key,const int64_t value,CFTypeID typeid){
//   assert(CFDictionaryContainsKey(checkInt_dict,key));
//   CFTypeRef r=CFDictionaryGetValue(checkInt_dict,key); // CFShow(r);
//   assert(CFGetTypeID(r)==CFNumberGetTypeID());
//   CFNumberRef n=r;
//   int64_t v=-1;
//   assert(CFNumberGetType(n)==kCFNumberSInt64Type&&
//          CFNumberGetByteSize(n)==64/8&&
//          CFNumberGetValue(n,kCFNumberSInt64Type,&v)&&
//          v==value);
// }

void cfd(){

  // https://developer.apple.com/documentation/corefoundation?language=objc
  // https://developer.apple.com/documentation/corefoundation/cfdictionary?language=objc
  CFDictionaryRef d=CFNetworkCopySystemProxySettings();
  assert(d);
  assert(CFDictionaryGetCount(d)==COUNT);

  checkInt_dict=d;
  /* [0] */ check1nt("HTTPEnable",+1); // HTTPEnable CFNumber +1
  /* [1] */ check1nt("HTTPPort",+8080);
  /* [2] */ check8tr("HTTPSProxy",PROXY_IP);
  /* [3] */ // check???("",);
  /* [4] */ check1nt("FTPPassive",+1);
  /* [5] */ check1nt("HTTPSPort",+8080);
  /* [6] */ check8tr("HTTPProxy",PROXY_IP);
  /* [7] */ check1nt("HTTPSEnable",+1);
  /* [8] */ // check???("",);

  // assert(CFDictionaryContainsKey(checkInt_dict,CFSTR("HTTPSProxy")));
  // CFTypeRef r=CFDictionaryGetValue(checkInt_dict,CFSTR("HTTPSProxy"));assert(r);
  // assert(CFGetTypeID(r)==CFStringGetTypeID());
  // CFStringRef s=r;
  // assert(strlen(PROXY_IP)==CFStringGetLength(s));
  // char buffer[SZ(PROXY_IP)];
  // bzero(buffer,SZ(PROXY_IP)*sizeof(char));
  // assert(CFStringGetCString(s,buffer,SZ(PROXY_IP),kCFStringEncodingASCII)&&
  //        !buffer[SZ(PROXY_IP)-1]&&
  //        0==strcmp(PROXY_IP,buffer));

  checkInt_dict=NULL;
  eprintf("\n");

  const void *keys[COUNT],*values[COUNT];
  bzero(keys,COUNT*sizeof(void*));
  bzero(values,COUNT*sizeof(void*));
  CFDictionaryGetKeysAndValues(d,keys,values);
  // CFDictionaryApplyFunction
  // for( const int *p=(int[]){0,1,2,3,4,5,6,7,8,-777}; *p>=0; ++p ){
  // for( const int *p=(int[]){2,3,6,8,-777}; *p>=0; ++p ){
  for( const int *p=(int[]){3,8,-777}; *p>=0; ++p ){
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
