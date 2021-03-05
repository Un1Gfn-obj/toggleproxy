[FreeBSD Manual Pages](https://www.freebsd.org/cgi/man.cgi)
<!-- [NEProxySettings](https://developer.apple.com/documentation/networkextension/neproxysettings) -->

https://stackoverflow.com/questions/8798699/i-can-get-the-system-proxy-with-objective-c-code-but-i-want-to-write-it-with-c\
https://stackoverflow.com/questions/36333784/programmatically-configure-proxy-settings-in-ios

PC

```plain
$ source ~/ios/ios.bashrc
$ make clean && rm -rfv packages/ .theos/ && make && ssh2 rm -fv \~/toggleproxy && scp2 .theos/obj/debug/toggleproxy && ssh2 \~/toggleproxy
```

iOS

```plain
./toggleproxy
```
