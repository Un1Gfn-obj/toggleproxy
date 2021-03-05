[FreeBSD Manual Pages](https://www.freebsd.org/cgi/man.cgi)
<!-- [NEProxySettings](https://developer.apple.com/documentation/networkextension/neproxysettings) -->

PC

```plain
$ source ~/ios/ios.bashrc
$ make clean && rm -rfv packages/ .theos/ && make && ssh2 rm -v \~/toggleproxy && scp2 .theos/obj/debug/toggleproxy
```

iOS

```plain
./toggleproxy
```

