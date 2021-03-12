// https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
#if defined __OBJC__ || defined __cplusplus
#error
#endif

#include "./c.h" // Self
#include "./secret.h"
#include "./privilege.h"

#include <netinet/in.h> // INET6_ADDRSTRLEN
#include <stdarg.h> // stdarg(3) one_man_army()
#include <CFNetwork/CFNetwork.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SCNetworkConfiguration.h>

// // /Library/Preferences: symbolic link to ../private/var/preferences
#define PLIST "/private/var/preferences/SystemConfiguration/preferences.plist"
#define UUIDSTRLEN 36
#define UUIDBYTES 16
#define ACOUNT 2

#define eprintf(...) fprintf(stderr,__VA_ARGS__)

static Status status=UNKNOWN;

/*static void c2cf_strequ(){

}*/

static int cf2c_strcmp(const char *const s,const CFStringRef theString){
  assert(theString&&CFGetTypeID(theString)==CFStringGetTypeID());
  const char *cString=CFStringGetCStringPtr(theString,kCFStringEncodingASCII);
  // https://developer.apple.com/documentation/corefoundation/1542133-cfstringgetcstringptr?language=objc#discussion
  if(cString){
    assert(cString[0]);
    return strcmp(s,cString);
  }else{
    CFIndex sz=strlen(s);
    ++sz;
    char buffer[sz];
    bzero(buffer,sz*sizeof(char));
    assert(CFStringGetCString(theString,buffer,sz,kCFStringEncodingASCII)&&
           !buffer[sz-1]);
    return strcmp(s,buffer);
  }
  assert(false);
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
static_assert(sizeof(int64_t)==sizeof(void*),"");
static_assert(sizeof(int64_t)==sizeof(CFTypeRef),"");

// https://www.ismp.org/resources/misidentification-alphanumeric-symbols
#define add1nt(D,K,V) add((CFMutableDictionaryRef)(D),K,          V ,CFNumberGetTypeID())
#define add8tr(D,K,V) add((CFMutableDictionaryRef)(D),K,(int64_t)(V),CFStringGetTypeID())
static void add(const CFMutableDictionaryRef dict,const char *const keyRaw,const int64_t valueRaw,const CFTypeID typeid){

  assert(keyRaw&&keyRaw[0]);
  const CFStringRef key=CFStringCreateWithCString(kCFAllocatorDefault,keyRaw,kCFStringEncodingASCII);
  assert(key&&CFGetTypeID(key)==CFStringGetTypeID());
  assert(dict&&CFGetTypeID(dict)==CFDictionaryGetTypeID()&&!CFDictionaryContainsKey(dict,key));

  CFTypeRef value=NULL;
  if(false){
    ;
  }else if(typeid==CFNumberGetTypeID()){
    value=CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt64Type,&valueRaw);
    int64_t tmp=-1;
    assert(value&&
           CFGetTypeID(value)==CFNumberGetTypeID()&&
           CFNumberGetType(value)==kCFNumberSInt64Type&&
           CFNumberGetByteSize(value)==64/8&&
           CFNumberGetValue(value,kCFNumberSInt64Type,&tmp)&&
           tmp==valueRaw);
  }else if(typeid==CFStringGetTypeID()){
    value=CFStringCreateWithCString(kCFAllocatorDefault,(const char*)valueRaw,kCFStringEncodingASCII);
    assert(value&&CFGetTypeID(value)==CFStringGetTypeID());
  }else{
    assert(false);
  }
  CFDictionaryAddValue(dict,key,value);

  CFRelease(key);
  CFRelease(value);

}

// https://www.ismp.org/resources/misidentification-alphanumeric-symbols
#define check1nt(D,K,V) check(D,false,K,          V ,CFNumberGetTypeID())
#define check8tr(D,K,V) check(D,false,K,(int64_t)(V),CFStringGetTypeID())
#define checkTyp(D,K,T) check(D,true ,K,          0 ,                T  ) // Check value type only
static CFTypeRef check(const CFDictionaryRef dict,const Boolean type_only,const char *const keyRaw,const int64_t value,const CFTypeID typeid){
  assert(keyRaw&&keyRaw[0]);
  const CFStringRef key=CFStringCreateWithCString(kCFAllocatorDefault,keyRaw,kCFStringEncodingASCII);
  assert(dict&&
         CFGetTypeID(dict)==CFDictionaryGetTypeID()&&
         CFDictionaryContainsKey(dict,key));
  const CFTypeRef r=CFDictionaryGetValue(dict,key);
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

static void checkEcp(const CFDictionaryRef dict){
  const CFStringRef key=CFSTR("ExceptionsList");
  assert(dict&&
         CFGetTypeID(dict)==CFDictionaryGetTypeID()&&
         CFDictionaryContainsKey(dict,key));
  const CFTypeRef r=CFDictionaryGetValue(dict,key);
  assert(r&&CFGetTypeID(r)==CFArrayGetTypeID());
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

static void check_on8_off2(const CFDictionaryRef dict,const int n){
  switch(n){
  case 8:
    check1nt(dict,"HTTPEnable",+1);
    check1nt(dict,"HTTPPort",PROXY_PORT);
    check8tr(dict,"HTTPProxy",PROXY_IP);
    check1nt(dict,"HTTPSEnable",+1);
    check1nt(dict,"HTTPSPort",PROXY_PORT);
    check8tr(dict,"HTTPSProxy",PROXY_IP);
     __attribute__((fallthrough));
  case 2:
    checkEcp(dict);
    check1nt(dict,"FTPPassive",+1);
    break;
  default:
    assert(false);
    break;
  }
}

static CFTypeRef cfd0(CFDictionaryRef dict,Boolean outer){
  assert(dict&&CFGetTypeID(dict)==CFDictionaryGetTypeID());
  CFIndex dcount=CFDictionaryGetCount(dict);
  #define C2 check_on8_off2(dict,2)
  #define C8 check_on8_off2(dict,8)
  if(outer)switch(dcount){
    case  0:status=DISCONNECTED ;return NULL;break;
    case  3:status=CONNECTED_OFF;         C2;break;
    case  9:status=CONNECTED_ON ;         C8;break;
    default:assert(false);                  ;break;
  }else switch(dcount){
    case  2:assert(status==CONNECTED_OFF);C2;break;
    case  8:assert(status==CONNECTED_ON) ;C8;break;
    default:assert(false);                  ;break;
  }
  #undef C8
  #undef C2
  return outer?checkTyp(dict,"__SCOPED__",CFDictionaryGetTypeID()):NULL;
}

Status proxy_status(){

  // L0 {}
  CFTypeRef r=CFNetworkCopySystemProxySettings();
  assert(r&&CFGetTypeID(r)==CFDictionaryGetTypeID());
  // CFShow(r);exit(0);

  // L1 {en0={}}
  const CFDictionaryRef middle=cfd0((CFDictionaryRef)r,true); // outer
  if(!middle){
    assert(status==DISCONNECTED);
    return status;
  }
  assert(CFDictionaryGetCount(middle)==1);

  // L2 {}
  assert(!cfd0(checkTyp(middle,"en0",CFDictionaryGetTypeID()),false));

  CFRelease(r);r=NULL;
  return status;

}

// geten0(NULL) - Return NULL
// geten0(&ARR) - Return SCNetworkInterfaceRef of en0 - Set ARR to a
static SCNetworkInterfaceRef geten0(CFArrayRef *rp){
  CFArrayRef a=SCNetworkInterfaceCopyAll();
  assert(CFGetTypeID(a)==CFArrayGetTypeID());
  assert(CFArrayGetCount(a)==3);
  Boolean found=false;
  SCNetworkInterfaceRef iface=NULL;
  for(CFIndex i=0;i<3;++i){
    iface=CFArrayGetValueAtIndex(a,i);
    assert(iface&&CFGetTypeID(iface)==SCNetworkInterfaceGetTypeID());
    // CFShow(iface);
    CFStringRef t=SCNetworkInterfaceGetInterfaceType(iface);
    if(0==cf2c_strcmp("IEEE80211",t)){
      assert(found==false);
      found=true;
      assert(0==cf2c_strcmp("en0",SCNetworkInterfaceGetBSDName(iface)));
      assert(0==cf2c_strcmp(HWADDR,SCNetworkInterfaceGetHardwareAddressString(iface)));
      assert(!SCNetworkInterfaceGetInterface(iface));
      assert(0==cf2c_strcmp("Wi-Fi",SCNetworkInterfaceGetLocalizedDisplayName(iface)));
      assert(!SCNetworkInterfaceGetConfiguration(iface));
      break;
      // CFShow(SCNetworkInterfaceGetExtendedConfiguration(iface,CFSTR("Proxies")));
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
      // break;
    }
  }
  // assert(SCPreferencesCommitChanges());
  assert(found&&iface);
  if(rp){
    *rp=a;
    return iface;
  }
  CFRelease(a);
  return NULL;
}

void check_iface(){
  geten0(NULL);
}

static void check_uuid(const char *const cStr){

  // check_uuid(""); // F
  // check_uuid("A"); // P Z
  // check_uuid("0A"); // P N
  // check_uuid("A0"); // P N
  // check_uuid("AA"); // P N
  // check_uuid("AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAA"); // P N
  // check_uuid("AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA"); // P N
  // check_uuid("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"); // P Z
  assert(cStr);
  for(int i=0;i<UUIDSTRLEN;++i)
    assert(cStr[i]);
  assert(!cStr[UUIDSTRLEN]);
  const CFStringRef s=CFStringCreateWithCString(kCFAllocatorDefault,cStr,kCFStringEncodingASCII);
  assert(s&&CFGetTypeID(s)==CFStringGetTypeID());
  const CFUUIDRef u=CFUUIDCreateFromString(kCFAllocatorDefault,s);
  CFRelease(s);
  // CFShow(u);

  // UUID parsable
  if(!( u&&CFGetTypeID(u)==CFUUIDGetTypeID() )){
    assert(false);
    // eprintf("F \n");
    // return;
  }
  // eprintf("P ");

  // UUID non-zero
  static_assert(sizeof(CFUUIDBytes)==UUIDBYTES,"");
  CFUUIDBytes bytes=CFUUIDGetUUIDBytes(u);
  CFRelease(u);
  const unsigned char *p=(unsigned char*)&bytes;

  if( !p[0] && 0==memcmp(p,p+1,(UUIDBYTES-1)) ){
    assert(false);
    // eprintf("Z \n");
    // return;
  }
  // eprintf("N \n");

}

// stdarg(3)
static CFDictionaryRef one_man_army(CFDictionaryRef r,...){
  va_list ap;
  va_start(ap,r);
  const char *k=NULL;
  while((k=(const char*)va_arg(ap,const char*))){
    r=checkTyp(r,k,CFDictionaryGetTypeID());
    assert(CFDictionaryGetCount(r)==1);
  }
  return r;
}

static CFURLRef path2url(const char *const path){
  CFStringRef s=CFStringCreateWithCString(kCFAllocatorDefault,path,kCFStringEncodingASCII);
  assert(s&&CFGetTypeID(s)==CFStringGetTypeID());
  CFURLRef url=CFURLCreateWithFileSystemPath(kCFAllocatorDefault,s,kCFURLPOSIXPathStyle,false);
  assert(url&&CFGetTypeID(url)==CFURLGetTypeID());
  CFRelease(s);s=NULL;
  return url;
}

static void cf_plist(const Boolean already_on){

  assert(0==access(PLIST,F_OK));
  assert(0==access(PLIST,R_OK));

  CFURLRef fileURL=path2url(PLIST);
  CFReadStreamRef rStream=CFReadStreamCreateWithFile(kCFAllocatorDefault,fileURL); // The URL must use the file scheme
  assert(rStream&&CFGetTypeID(rStream)==CFReadStreamGetTypeID());
  CFRelease(fileURL);fileURL=NULL;

  assert(CFReadStreamOpen(rStream));
  CFPropertyListFormat format=-1;
  CFErrorRef error=NULL;
  const CFPropertyListRef plist=CFPropertyListCreateWithStream(
    kCFAllocatorDefault,
    rStream,
    0,
    kCFPropertyListMutableContainersAndLeaves,
    &format,
    &error);
  assert(!error&&
         plist&&
         format==kCFPropertyListBinaryFormat_v1_0);
  assert(CFPropertyListIsValid(plist,kCFPropertyListBinaryFormat_v1_0));

  CFReadStreamClose(rStream);CFRelease(rStream);rStream=NULL;

  // Dump
  assert(CFGetTypeID(plist)==CFDictionaryGetTypeID());
  const CFDictionaryRef root=plist;
  // CFShow(root);

  // Begin modification (pseudocode plist.txt)

  check8tr(root,"Model","J96AP");
  check1nt(root,"__VERSION__",20191120);

  // $uuidSet
  const char *const prefix="/Sets/";
  const size_t prefixL=strlen(prefix);
  const char *uuidSet=CFStringGetCStringPtr(checkTyp(root,"CurrentSet",CFStringGetTypeID()),kCFStringEncodingASCII);
  assert(uuidSet&&0==strncmp(prefix,uuidSet,prefixL));
  check_uuid((uuidSet+=prefixL));

  // $dSet
  const CFDictionaryRef dSet=checkTyp(checkTyp(root,
    "Sets",CFDictionaryGetTypeID()),
    uuidSet,CFDictionaryGetTypeID());
  check8tr(dSet,"UserDefinedName",SSID);

  // $pNetwork
  const CFDictionaryRef pNetwork=checkTyp(dSet,"Network",CFDictionaryGetTypeID());
  checkTyp(one_man_army(pNetwork,"Interface","en0","AirPort",NULL),"JoinMode",CFStringGetTypeID());
  const CFArrayRef srvOrd=checkTyp(one_man_army(pNetwork,"Global","IPv4",NULL),"ServiceOrder",CFArrayGetTypeID());

  // $uuidNS
  assert(CFArrayGetCount(srvOrd)==5);
  const CFStringRef uuidNSboxed=CFArrayGetValueAtIndex(srvOrd,2);
  assert(uuidNSboxed&&CFGetTypeID(uuidNSboxed)==CFStringGetTypeID());
  const char *const uuidNS=CFStringGetCStringPtr(uuidNSboxed,kCFStringEncodingASCII);
  check_uuid(uuidNS);

  // $dNS
  const CFDictionaryRef dNS=checkTyp(checkTyp(root,
    "NetworkServices",CFDictionaryGetTypeID()),
    uuidNS,CFDictionaryGetTypeID());

  // $dProxies
  const CFDictionaryRef dInterface=checkTyp(dNS,"Interface",CFDictionaryGetTypeID());
  assert(CFDictionaryGetCount(dInterface)==4);
  check8tr(dInterface,"DeviceName"     ,"en0"     );
  check8tr(dInterface,"Hardware"       ,"AirPort" );
  check8tr(dInterface,"Type"           ,"Ethernet");
  check8tr(dInterface,"UserDefinedName","Wi-Fi"   );
  const CFDictionaryRef dProxies=checkTyp(dNS,"Proxies",CFDictionaryGetTypeID());

  if(already_on){

    // on -> off
    assert(CFDictionaryGetCount(dProxies)==8);
    check_on8_off2(dProxies,8);
    for(const char **sp=(const char*[]){
      "HTTPEnable",
      "HTTPPort",
      "HTTPProxy",
      "HTTPSEnable",
      "HTTPSPort",
      "HTTPSProxy",
      NULL}; *sp; ++sp
    ){
      const CFStringRef key=CFStringCreateWithCString(kCFAllocatorDefault,*sp,kCFStringEncodingASCII);
      assert(CFDictionaryContainsKey(dProxies,key));
      assert(1==CFDictionaryGetCountOfKey(dProxies,key));
      CFDictionaryRemoveValue((CFMutableDictionaryRef)dProxies,key);
      assert(!CFDictionaryContainsKey(dProxies,key));
      assert(0==CFDictionaryGetCountOfKey(dProxies,key));
      CFRelease(key);
    }
    assert(CFDictionaryGetCount(dProxies)==2);
    check_on8_off2(dProxies,2);

  }else{

    // off -> on
    assert(CFDictionaryGetCount(dProxies)==2);
    check_on8_off2(dProxies,2);
    add1nt(dProxies,"HTTPEnable",+1);
    add1nt(dProxies,"HTTPPort",PROXY_PORT);
    add8tr(dProxies,"HTTPProxy",PROXY_IP);
    add1nt(dProxies,"HTTPSEnable",+1);
    add1nt(dProxies,"HTTPSPort",PROXY_PORT);
    add8tr(dProxies,"HTTPSProxy",PROXY_IP);
    assert(CFDictionaryGetCount(dProxies)==8);
    check_on8_off2(dProxies,8);

  }

  // End modification (pseudocode plist.txt)
  assert(CFPropertyListIsValid(plist,kCFPropertyListBinaryFormat_v1_0));

  // Write
  // fileURL=path2url("/private/var/mobile/tmp.plist");
  fileURL=path2url(PLIST);
  CFWriteStreamRef wStream=CFWriteStreamCreateWithFile(kCFAllocatorDefault,fileURL);
  assert(wStream&&CFGetTypeID(wStream)==CFWriteStreamGetTypeID());
  privilege_escalate();
  assert(0==geteuid());
  (0==getuid())?assert(0==access(PLIST,W_OK)):0; // W_OK for root, fails for mobile+setuid
  assert(CFWriteStreamOpen(wStream));
  assert(1<=CFPropertyListWrite(
    plist,
    wStream,
    kCFPropertyListBinaryFormat_v1_0,
    0,
    &error));
  assert(!error);
  CFWriteStreamClose(wStream);CFRelease(wStream);wStream=NULL;
  privilege_drop();
  CFRelease(fileURL);fileURL=NULL;

  // fileURL=path2url("/private/var/mobile/tmp_cfdata.plist");
  // wStream=CFWriteStreamCreateWithFile(kCFAllocatorDefault,fileURL);
  // assert(wStream&&CFGetTypeID(wStream)==CFWriteStreamGetTypeID());
  // assert(CFWriteStreamOpen(wStream));
  // CFDataRef cfdata=CFPropertyListCreateData(
  //   kCFAllocatorDefault,
  //   plist,
  //   kCFPropertyListBinaryFormat_v1_0,
  //   0,
  //   &error);
  // assert(!error);
  // assert(cfdata&&CFGetTypeID(cfdata)==CFDataGetTypeID());
  // // Deprecated
  // // Boolean CFURLWriteDataAndPropertiesToResource(CFURLRef url, CFDataRef dataToWrite, CFDictionaryRef propertiesToWrite, SInt32 *errorCode);
  // CFWriteStreamClose(wStream);
  // CFRelease(fileURL);fileURL=NULL;

  // Dealloc
  CFRelease(plist);

}

void on2off(){
  cf_plist(true);
}

void off2on(){
  cf_plist(false);
}

// https://iphonedevwiki.net/index.php/MobileWiFi.framework
// https://github.com/davidmurray/ios-reversed-headers
// #include "ios-reversed-headers/MobileWiFi/MobileWiFi.h"
/*void cf_force_refresh(){

  // No effect
  CFArrayRef arr=NULL;
  const SCNetworkInterfaceRef en0=geten0(&arr);
  assert(arr&&en0&&0==cf2c_strcmp("en0",SCNetworkInterfaceGetBSDName(en0)));
  assert(SCNetworkInterfaceForceConfigurationRefresh(en0));
  CFRelease(arr);

  // const WiFiManagerRef manager=WiFiManagerClientCreate(kCFAllocatorDefault,0);
  // CFShow(manager);
  // const CFArrayRef devices=WiFiManagerClientCopyDevices(manager);
  // CFShow(devices);
  // eprintf("%ld\n",CFArrayGetCount(devices));
  // eprintf("\n");
  // CFRelease(devices);
  // CFRelease(manager);

}*/
