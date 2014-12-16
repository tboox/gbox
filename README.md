The Graphic Box Library
========================

GBOX is a mutli-platform c graphic library, ui and game engine for unix, windows, mac, ios, android, etc.

contact
-------

- email:   	    waruqi@gmail.com
- source:  	    https://github.com/waruqi/gbox
- website: 	    http://www.tboox.org or http://www.tboox.net
- download:     https://github.com/waruqi/gbox/archive/master.zip
- qq(group):    343118190

donate
------

####alipay
<img src="res/donate/alipay.png" alt="alipay" width="128" height="128">

build
-----

```bash
	// build for linux-x86 debug
    cd ./gbox
    [make f PLAT=linux ARCH=x86 DEBUG=y] => optional
    make r

	// build for linux-x64 release
    cd ./gbox
    [make f PLAT=linux ARCH=x64 DEBUG=n] => optional
    make r

	// build for linux and add cflags and ldflags
    cd ./gbox
    make f PLAT=linux CFLAG="-I." LDFLAG="-L. -lxxx"
    make r

	// build for mac
    cd ./gbox
    [make f PLAT=mac ARCH=x64] => optional
    make r

	// build for ios-armv7, using sdk7.1 framework
    cd ./gbox
    make f PLAT=ios ARCH=armv7 SDK=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.1.sdk
    make r
    
	// build for ios armv7 and arm64 universal version
    cd ./gbox
    make lipo ARCH1=armv7 ARCH2=arm64 SDK=/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.1.sdk

	// build for android-armv5te, need set the ndk and toolchains directory
    cd ./gbox
    make f PLAT=android ARCH=armv5te SDK=/home/xxxx/android-ndk-r9d BIN=/home/xxxx/android-ndk-r9d/toolchains/arm-linux-androideabi-4.8/prebuilt/darwin-x86_64/bin
    make r
    
	// build for android-armv6, if ndk and toolchains have been setted
    cd ./gbox
    make f PLAT=android ARCH=armv6
    make r

	// build for windows using msvc
    cd ./gbox
    run ./msys.bat
    [make f PLAT=msvc ARCH=x86] => optional
    make r

	// build for windows using mingw and need link libgcc.a from mingw
    run msys
    cd ./gbox
    [make f PLAT=mingw ARCH=x86] => optional
    make r

	// build for windows using cygwin 
    run cygwin
    cd ./gbox
    [make f PLAT=cygwin ARCH=x86] => optional
    make r

	// build for windows and custom complier path and prefix and need link libgcc.a from mingw
    run cygwin
    cd ./gbox
    make f PLAT=mingw ARCH=x86 BIN="/home/xxx/bin" PRE="i386-mingw32-"
    make r
```

