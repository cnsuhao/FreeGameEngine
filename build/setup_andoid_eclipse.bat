@echo off

echo ANDROID_NDK: %ANDROID_NDK%

if not exist %ANDROID_NDK%\ndk-build (
	echo error ANDROID_NDK not found!.
	goto end
)

set cmd_path=%~dp0
set root_path=%cmd_path%..\
set work_path="build.android.eclipse"

echo "root_path: %root_path%"

pushd %root_path%

if not exist %work_path% mkdir %work_path%
cd %work_path%

cmake.exe -G"Eclipse CDT4 - Ninja" -DANDROID_NDK=%ANDROID_NDK% -DANDROID_STL=gnustl_static -DANDROID_NATIVE_API_LEVEL=android-14 -DCMAKE_TOOLCHAIN_FILE=%root_path%\CMake\Toolchains\android.toolchain.cmake -DCMAKE_MAKE_PROGRAM=ninja -DCLIENT=true -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_VERSION="4.2" ..

popd

:end
pause
