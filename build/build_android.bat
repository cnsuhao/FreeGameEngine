@echo off

echo ANDROID_NDK: %ANDROID_NDK%

if not exist %ANDROID_NDK%\ndk-build (
	echo error ANDROID_NDK not found!.
	goto end
)

set cmd_path=%~dp0
set root_path=%cmd_path%..\

echo "root_path: %root_path%"

pushd %root_path%\build.android.ninja


set PATH=%PATH%;%ANDROID_NDK%;%ANDROID_SDK%;%ANDROID_SDK%\tools;%ANDROID_SDK%\platform-tools;%ANDROID_ANT%

ninja.exe -j 4

popd

:end
pause
