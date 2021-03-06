
https://gcc.gnu.org/onlinedocs/cpp/Macro-Pitfalls.html#Macro-Pitfalls

[FreeBSD Manual Pages](https://www.freebsd.org/cgi/man.cgi)

https://stackoverflow.com/questions/8798699/i-can-get-the-system-proxy-with-objective-c-code-but-i-want-to-write-it-with-c\

https://stackoverflow.com/questions/36333784/programmatically-configure-proxy-settings-in-ios

PC

```plain
$ source ~/ios/ios.bashrc
$ make purge all && ssh2 rm -fv \~/toggleproxy && scp2 .theos/obj/debug/toggleproxy && ssh2 \~/toggleproxy
```

https://lldb.llvm.org/use/map.html

```plain
bt all
frame select 2
source list
```

CFShow() -
CFCopyDescription() -
CFCopyTypeIDDescription(CFGetTypeID(r)) -
CFRelease()

[Global Proxy Settings Constants](https://developer.apple.com/documentation/cfnetwork/global_proxy_settings_constants?language=objc) not available in iOS

```
cfd.m:nn:nn: error: 'kCFNetworkProxiesFTPPassive' is unavailable: not available on iOS
  const CFTypeRef r=CFDictionaryGetValue(globCurDict,kCFNetworkProxiesFTPPassive);assert(r);
                                                     ^
/opt/theos/sdks/iPhoneOS14.4.sdk/System/Library/Frameworks/CFNetwork.framework/Headers/CFProxySupport.h:nnn:nn: note: 'kCFNetworkProxiesFTPPassive' has been explicitly marked unavailable here
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPPassive CF_AVAILABLE(10_6, NA);
                             ^
```
