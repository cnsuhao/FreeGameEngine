Hello freegame

a cross-platform game engine, not complete.
Copyright (c) youlanhai. All rights reserved.

started at 2015.6.1

--------------------------------------------------
# how to build
==================================================

## 1. requirements
cmake2.8 or above is required.
**windows**: visual studio 2013.
**mac & ios**: xcode.
**android**: sdk, ndk, ant.

## 2. windows
run `build/setup_win32.bat`,
then open `build.win32/freegame.sln`

## 3. mac
run `build/setup_mac.sh`,
then open `build.mac/freegame.xcodeproj`

## 4. ios
run `build/setup_ios.sh`
then open `game/yuefe/proj.ios/client.xcodeproj`

## 5. android
set the environment variable:

    ANDROID_NDK = D:\develop\andriod\android-ndk-r9d
    ANDROID_SDK = D:\develop\andriod\sdk
    ANDROID_ANT = D:\develop\apache-ant-1.9.3\bin

set the envrionment path:

	PATH=%PATH%;%ANDROID_NDK%;%ANDROID_SDK%\tools;%ANDROID_SDK%\platform-tools;%ANDROID_ANT%

1. run `build/setup_andoid.bat`, to generate the project files.
2. run `build/build_android.bat`, to build the dynamic c++ library.
3. enter into directory "game/client/proj.android"
4. update android project: `android update project -p . -t 1`
5. build apk: `ant debug`
6. start the adb service: `adb start-server`
7. connect your mobile by useb, then run `adb install bin/SDLActivity-debug.apk` to install application. 
8. run `adb shell am com.SDL.SDLActivity` to start application.
