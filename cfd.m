#include <netinet/in.h> // INET6_ADDRSTRLEN
#include "./secret.h"

#import <Foundation/Foundation.h>
// #import <NetworkExtension/NEProxySettings.h>

#define DCOUNT 9
#define ACOUNT 2
#define eprintf(...) fprintf(stderr,__VA_ARGS__)

// Use "Global Proxy Settings Constants" instead?
// https://developer.apple.com/documentation/cfnetwork/global_proxy_settings_constants?language=objc
static CFDictionaryRef check_dict=NULL;

/*static void c2cf_strequ(){

}*/

static void cf2c_strequ(const CFStringRef theString,const char *const s){
  CFIndex sz=strlen(s);
  assert(sz==CFStringGetLength(theString));
  ++sz;
  char buffer[sz];
  bzero(buffer,sz*sizeof(char));
  // assert(CFStringGetCString(theString,buffer,sz,kCFStringEncodingASCII));
  // assert(!buffer[sz-1]);
  // assert(0==strcmp(s,buffer));
  assert(CFStringGetCString(theString,buffer,sz,kCFStringEncodingASCII)&&
         !buffer[sz-1]&&
         0==strcmp(s,buffer));
}

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

// https://www.ismp.org/resources/misidentification-alphanumeric-symbols
#define check1nt(K,V) check(false,CFSTR(K),          V ,CFNumberGetTypeID())
#define check8tr(K,V) check(false,CFSTR(K),(int64_t)(V),CFStringGetTypeID())
#define checkTyp(K,T) check(true ,CFSTR(K),          0 ,                T  ) // Check value type only
static void check(bool type_only,CFStringRef key,const int64_t value,CFTypeID typeid){
  assert(CFDictionaryContainsKey(check_dict,key));
  const CFTypeRef r=CFDictionaryGetValue(check_dict,key); // CFShow(r);
  assert(r&&CFGetTypeID(r)==typeid);
  // if(CFGetTypeID(r)!=typeid){
  //   CFStringRef s=CFCopyTypeIDDescription(CFGetTypeID(r));
  //   CFShow(s);
  //   CFRelease(s);
  //   assert(false);
  // }
  if(type_only){
    assert(value==0);
  }else if(typeid==CFNumberGetTypeID()){
    const CFNumberRef n=r;
    int64_t v=-1;
    assert(CFNumberGetType(n)==kCFNumberSInt64Type&&
           CFNumberGetByteSize(n)==64/8&&
           CFNumberGetValue(n,kCFNumberSInt64Type,&v)&&
           v==value);
  }else if(typeid==CFStringGetTypeID()){
    cf2c_strequ((CFStringRef)r,(const char*)value);
    // // Shadowing causes segfault, use a different identifier
    // // const char *value=(const char*)value;
  }else{
    assert(false);
  }
  CFRelease(key);
}

#define checkEcp(K) checkEcp0(CFSTR(K))
static void checkEcp0(CFStringRef key){
  const CFTypeRef r=CFDictionaryGetValue(check_dict,key);assert(r);
  assert(CFGetTypeID(r)==CFArrayGetTypeID());
  CFArrayRef a=r;
  assert(CFArrayGetCount(a)==ACOUNT);
  const char * const exceptions[ACOUNT]={"*.local","169.254/16"};
  for(CFIndex i=0;i<ACOUNT;++i){
    CFTypeRef rr=CFArrayGetValueAtIndex(a,i);
    assert(rr&&CFGetTypeID(rr)==CFStringGetTypeID());
    cf2c_strequ((CFStringRef)rr,exceptions[i]);
  }
  CFRelease(key);
}

void cfd(){

  // https://developer.apple.com/documentation/corefoundation?language=objc
  // https://developer.apple.com/documentation/corefoundation/cfdictionary?language=objc
  CFDictionaryRef d=CFNetworkCopySystemProxySettings();
  assert(d);
  // CFShow(d);/*exit(0);*/
  assert(CFDictionaryGetCount(d)==DCOUNT);

  check_dict=d;
  /* [0] */ check1nt("HTTPEnable",+1); // HTTPEnable CFNumber +1
  /* [1] */ check1nt("HTTPPort",+8080);
  /* [2] */ check8tr("HTTPSProxy",PROXY_IP);
  /* [3] */ checkEcp("ExceptionsList");
  /* [4] */ check1nt("FTPPassive",+1);
  /* [5] */ check1nt("HTTPSPort",+8080);
  /* [6] */ check8tr("HTTPProxy",PROXY_IP);
  /* [7] */ check1nt("HTTPSEnable",+1);
  /* [8] */ checkTyp("__SCOPED__",CFDictionaryGetTypeID());
  check_dict=NULL;
  eprintf("\n");

  // const void *keys[DCOUNT],*values[DCOUNT];
  // bzero(keys,DCOUNT*sizeof(void*));
  // bzero(values,DCOUNT*sizeof(void*));
  // CFDictionaryGetKeysAndValues(d,keys,values);
  // // CFDictionaryApplyFunction
  // // for( const int *p=(int[]){0,1,2,3,4,5,6,7,8,-777}; *p>=0; ++p ){
  // // for( const int *p=(int[]){2,3,6,8,-777}; *p>=0; ++p ){
  // for( const int *p=(int[]){3,8,-777}; *p>=0; ++p ){
  //   assert(keys[*p]&&values[*p]);
  //   fprintf(stderr,"[%d] ",*p);CFShow(keys[*p]);
  //   CFShow(values[*p]);
  //   fprintf(stderr,"\n");
  // }

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
