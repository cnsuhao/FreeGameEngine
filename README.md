Hello freegame

orange
a cross-platform game engine.
Copyright (c) 2014 jingjie(http://www.363.com). All rights reserved.

started at 2014.6.3.

一.编译

需要cmake2.8以上。
windows: visual studio 2013.
mac & ios: xcode.
android: sdk, ndk, ant.

########################################################################
### windows
########################################################################
1.执行build/setup_win32.bat。
2.打开build.win32/orange_client.sln

########################################################################
### ios
########################################################################
1.执行build/setup_ios.sh
2.打开game/yuefe/proj.ios/client.xcodeproj

########################################################################
### android
########################################################################
设置path环境变量，需要加的路径如下：
ndk：	ANDROID_NDK = D:\develop\andriod\android-ndk-r9d
sdk:	ANDROID_SDK = D:\develop\andriod\sdk
ant: 	ANDROID_ANT = D:\develop\apache-ant-1.9.3\bin

设置path：
	ANDROID_NDK
	ANDROID_SDK\tools
	ANDROID_SDK\platform-tools
	ANDROID_ANT

编译c++
1.执行build/setup_andoid.bat，生成工程文件。
2.执行build/build_android.bat，编译c++，生成动态链接库，可供java调用。

进入目录game/yuefe/proj.android
1.第一次编译，先执行android update project -p . -t 1
2.启动adb服务：adb start-server
3.usb连接android设备，开启usb调试模式（推荐真机调试，模拟器实在太慢）
4.生成apk：ant debug
5.安装：adb install bin/SDLActivity-debug.apk

4、5步骤可以通过eclipse来完成。

########################################################################
### mac
########################################################################
