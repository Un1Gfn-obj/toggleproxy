// Make sure this file is not compiled as an Objetive-C source
// Expect failure
// #import <Foundation/Foundation.h>
// static void test(){
//   [NSString new];
// }

#include <netinet/in.h> // INET6_ADDRSTRLEN

#include "./secret.h"
#include "./status.h"

#include <CoreFoundation/CoreFoundation.h>

#include <CFNetwork/CFNetwork.h>
#include <SystemConfiguration/SCNetworkConfiguration.h>

#define ACOUNT 2
#define eprintf(...) fprintf(stderr,__VA_ARGS__)

static CFDictionaryRef globCurDict=NULL;

static Status status=UNKNOWN;

/*static void c2cf_strequ(){

}*/

static int cf2c_strcmp(const char *const s,const CFStringRef theString){
  CFIndex sz=strlen(s);
  // assert(sz==CFStringGetLength(theString));
  ++sz;
  char buffer[sz];
  bzero(buffer,sz*sizeof(char));
  assert(CFStringGetCString(theString,buffer,sz,kCFStringEncodingASCII));
  assert(!buffer[sz-1]);
  return strcmp(s,buffer);
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
static CFTypeRef check(Boolean type_only,CFStringRef key,const int64_t value,CFTypeID typeid){
  assert(CFDictionaryContainsKey(globCurDict,key));
  const CFTypeRef r=CFDictionaryGetValue(globCurDict,key); // CFShow(r);
  assert(r&&CFGetTypeID(r)==typeid);
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
    // Shadowing causes segfault, use a different identifier
    // const char *value=(const char*)value;
    assert(0==cf2c_strcmp((const char*)value,(CFStringRef)r));
  }else{
    assert(false);
  }
  CFRelease(key);
  return r;
}

#define checkEcp(K) checkEcp0(CFSTR(K))
static void checkEcp0(CFStringRef key){
  const CFTypeRef r=CFDictionaryGetValue(globCurDict,key);assert(r);
  assert(CFGetTypeID(r)==CFArrayGetTypeID());
  CFArrayRef a=r;
  assert(CFArrayGetCount(a)==ACOUNT);
  const char * const exceptions[ACOUNT]={"*.local","169.254/16"};
  for(CFIndex i=0;i<ACOUNT;++i){
    CFTypeRef rr=CFArrayGetValueAtIndex(a,i);
    assert(rr&&CFGetTypeID(rr)==CFStringGetTypeID());
    assert(0==cf2c_strcmp(exceptions[i],(CFStringRef)rr));
  }
  CFRelease(key);
}

static CFTypeRef cfd0(Boolean outer){

  CFIndex dcount=CFDictionaryGetCount(globCurDict);

  if(outer)switch(dcount){
    case  0:status=DISCONNECTED ;return NULL;break;
    case  3:status=CONNECTED_OFF            ;break;
    case  9:status=CONNECTED_ON             ;break;
    default:assert(false);                  ;break;
  }else switch(dcount){
    case  2:assert(status==CONNECTED_OFF)   ;break;
    case  8:assert(status==CONNECTED_ON)    ;break;
    default:assert(false);                  ;break;
  }

  /*                  */ // eprintf("A\n");
  /*                  */ checkEcp("ExceptionsList");
  /*                  */ check1nt("FTPPassive",+1);
  (status==CONNECTED_ON)?check1nt("HTTPEnable",+1):0; // HTTPEnable CFNumber +1
  (status==CONNECTED_ON)?check1nt("HTTPSEnable",+1):0;
  (status==CONNECTED_ON)?check1nt("HTTPPort",+8080):0;
  (status==CONNECTED_ON)?check1nt("HTTPSPort",+8080):0;
  (status==CONNECTED_ON)?check8tr("HTTPProxy",PROXY_IP):0;
  (status==CONNECTED_ON)?check8tr("HTTPSProxy",PROXY_IP):0;
  /*                  */ // eprintf("B\n");
  return outer?checkTyp("__SCOPED__",CFDictionaryGetTypeID()):NULL;

  // const void *keys[dcount],*values[dcount];
  // bzero(keys,dcount*sizeof(void*));
  // bzero(values,dcount*sizeof(void*));
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

}

Status proxy_status(){

  // https://developer.apple.com/documentation/corefoundation?language=objc
  CFTypeRef r=CFNetworkCopySystemProxySettings();
  assert(r&&CFGetTypeID(r)==CFDictionaryGetTypeID());
  // CFShow(r);exit(0);

  // globCurDict <- Outer L0 {}
  globCurDict=(CFDictionaryRef)r;

  // globCurDict <- Middle L1 {en0={}}
  globCurDict=cfd0(true); // outer
  if(!globCurDict){
    assert(status==DISCONNECTED);
    return status;
  }
  assert(CFDictionaryGetCount(globCurDict)==1);

  // globCurDict <- Inner L2 {}
  globCurDict=checkTyp("en0",CFDictionaryGetTypeID());

  // globCurDict <- NULL
  globCurDict=cfd0(false); // inner
  assert(!globCurDict);

  CFRelease(r);r=NULL;
  return status;

}

void check_iface(){

  CFArrayRef a=SCNetworkInterfaceCopyAll();
  assert(CFGetTypeID(a)==CFArrayGetTypeID());

  assert(CFArrayGetCount(a)==3);
  Boolean found=false;
  for(CFIndex i=0;i<3;++i){

    SCNetworkInterfaceRef iface=CFArrayGetValueAtIndex(a,i);
    assert(CFGetTypeID(iface)==SCNetworkInterfaceGetTypeID());
    // CFShow(iface);

    CFStringRef t=SCNetworkInterfaceGetInterfaceType(iface);
    if(0==cf2c_strcmp("IEEE80211",t)){

      assert(found==false);
      found=true;

      assert(0==cf2c_strcmp("en0",SCNetworkInterfaceGetBSDName(iface)));
      assert(0==cf2c_strcmp(HWADDR,SCNetworkInterfaceGetHardwareAddressString(iface)));
      assert(!SCNetworkInterfaceGetInterface(iface));
      assert(0==cf2c_strcmp("Wi-Fi",SCNetworkInterfaceGetLocalizedDisplayName(iface)));

      // SCNetworkInterfaceSetConfiguration(iface, CFDictionaryRef __nullable config)

      // CFShow(SCNetworkInterfaceGetSupportedInterfaceTypes(iface)); // PPP
      // CFShow(SCNetworkInterfaceGetSupportedProtocolTypes(iface)); // DNS IPv4 IPv6 Proxies

      // CFDictionaryRef current=NULL;
      // CFDictionaryRef active=NULL;
      // CFArrayRef available=NULL;
      // assert(SCNetworkInterfaceCopyMediaOptions(iface,
      //   &current,
      //   &active,
      //   &available,
      //   false
      // ));
      // CFShow(current);
      // CFShow(active);
      // CFShow(available);

      break;
    }

    // CFShow(SCNetworkInterfaceGetConfiguration(iface));

  }

  // assert(SCPreferencesCommitChanges());
  assert(found);
  CFRelease(a);

}

/*static void nsd(){@autoreleasepool{
  NSDictionary *nsd=(__bridge NSDictionary*)CFNetworkCopySystemProxySettings();
  NSDictionary *nsd=CFBridgingRelease(CFNetworkCopySystemProxySettings());
  // https://developer.apple.com/documentation/networkextension/neproxysettings?language=occ
  NEProxySettings *s=[NEProxySettings new];
  printf("%d\n",s.HTTPEnabled);
  printf("%d\n",proxySettings.HTTPEnabled);
}}*/
