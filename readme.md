[GCC Macro Pitfalls](https://gcc.gnu.org/onlinedocs/cpp/Macro-Pitfalls.html#Macro-Pitfalls)

[Mac OS X Manual Pages](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/printf.3.html)
<s>[FreeBSD Manual Pages](https://www.freebsd.org/cgi/man.cgi)</s>

[PropertyList-1.0.dtd](https://www.apple.com/DTDs/PropertyList-1.0.dtd)

[MoreSCF](https://developer.apple.com/legacy/mac/library/#samplecode/MoreSCF/Introduction/Intro.html) -
[Download Sample Code](https://developer.apple.com/library/archive/samplecode/MoreSCF/Introduction/Intro.html#Sample_button)

---

```objc
CFCopyDescription()
CFCopyTypeIDDescription(CFGetTypeID(r))
CFRelease()
```

[System Configuration Framework](https://developer.apple.com/documentation/systemconfiguration?language=objc) -
[Global Proxy Settings Constants](https://developer.apple.com/documentation/cfnetwork/global_proxy_settings_constants?language=objc) not available in iOS

```
cfd.m:nn:nn: error: 'kCFNetworkProxiesFTPPassive' is unavailable: not available on iOS
  const CFTypeRef r=CFDictionaryGetValue(globCurDict,kCFNetworkProxiesFTPPassive);assert(r);
                                                     ^
/opt/theos/sdks/iPhoneOS14.4.sdk/System/Library/Frameworks/CFNetwork.framework/Headers/CFProxySupport.h:nnn:nn: note: 'kCFNetworkProxiesFTPPassive' has been explicitly marked unavailable here
CFN_EXPORT const CFStringRef kCFNetworkProxiesFTPPassive CF_AVAILABLE(10_6, NA);
                             ^
```

---

```plain
[darren@... toggleproxy]$ source ~/ios/ios.bashrc
[darren@... toggleproxy]$ make purge all transfer
```

```plain
iPad-...-...:~ mobile$ echo && sha1sum ./toggleproxy && ./toggleproxy
```

https://lldb.llvm.org/use/map.html

```plain
bt all
frame select 2
source list
```

<!-- SpringBoard - Settings - WLAN - i - HTTP PROXY - Configure Proxy - Manual - Server - 7.21.8.31\
SpringBoard - Settings - WLAN - i - HTTP PROXY - Configure Proxy - Manual - Port - 8964 -->

```bash
source ~/ios/ios.bashrc
cd /tmp
scp3 /private/var/preferences
find preferences -type f | xargs strings -af | grep -F -e "7.21.8.31" -e "8964"
# cd preferences
# shopt -s globstar
# strings -af ** | grep "7.21.8.31"
# strings -af ** | grep "8964"
cd /tmp/preferences/SystemConfiguration/
plistutil -i preferences.plist -o preferences.plist.xml -f xml && subl preferences.plist.xml
plistutil -i preferences.plist -o - -f xml | grep -e "7.21.8.31" -e "8964"
```

---

https://discussions.apple.com/thread/252531803 \
https://developer.apple.com/forums/thread/65416

stackoverflow -
[**10671542**](https://stackoverflow.com/a/10671542/) -
[3464757](https://stackoverflow.com/a/3464757/) -
[36127254](https://stackoverflow.com/q/36127254/) -
[8798699](https://stackoverflow.com/q/8798699/) -
[<s>36333784</s>](https://stackoverflow.com/q/36333784/)

[ProxySetter.m](https://github.com/chetan51/sidestep/blob/master/ProxySetter.m)

[Documentation Archive](https://developer.apple.com/library/archive/navigation/) (read in firefox reader view) \
[How would I enable macOS System Proxy?](https://developer.apple.com/forums/thread/74672) -
[**System Configuration Framework**](https://developer.apple.com/library/archive/documentation/Networking/Conceptual/SystemConfigFrameworks/) \
[PropertyLists](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/PropertyLists/) -
[UserDefaults](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/UserDefaults/) -
[CFPreferences](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFPreferences/) -
[CFPropertyLists](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFPropertyLists/) -
[AppSandbox](https://developer.apple.com/library/archive/documentation/Security/Conceptual/AppSandboxDesignGuide/)
([brief](https://cocoacasts.com/what-is-application-sandboxing/))
([App Sandbox](https://developer.apple.com/documentation/security/app_sandbox?language=objc))

/opt/theos/sdks/iPhoneOS14.4.sdk/System/Library/Frameworks/SystemConfiguration.framework/Headers/SCSchemaDefinitions.h

```plain
... kSCPropNetProxiesHTTPEnable ... "HTTPEnable" ... CFNumber (0 or 1)
```

[NSDictionary:valueForKey](https://developer.apple.com/documentation/foundation/nsdictionary/1410210-valueforkey?language=objc) -
[key path](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/KeyValueCoding/BasicPrinciples.html) -
[SO](https://stackoverflow.com/a/4317864/8243991)

[cephei `/usr/bin/defaults`](https://hbang.github.io/libcephei/)

[SCNetworkSetCopyServices](https://developer.apple.com/documentation/systemconfiguration/1517085-scnetworksetcopyservices?language=objc) -
[SCPreferencesCommitChanges](https://developer.apple.com/documentation/systemconfiguration/1517333-scpreferencescommitchanges?language=occ) -
[SCPreferencesApplyChanges](https://developer.apple.com/documentation/systemconfiguration/1517125-scpreferencesapplychanges?language=occ) -
[SCPreferencesCreateWithAuthorization](https://developer.apple.com/documentation/systemconfiguration/1516686-scpreferencescreatewithauthoriza?language=objc)

[Paper Plane Icon](https://www.iconfinder.com/icons/3592673/align_center_flip2_graphic_mirro_tool_vertical_icon)

[TheCreateRule](https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html#//apple_ref/doc/writerid/cfCreateRule)

https://whatismyipaddress.com
