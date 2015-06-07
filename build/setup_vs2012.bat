@echo off

call export_lua.bat

pushd %~dp0..\

echo current path: %cd%

if not exist "build.win32" mkdir "build.win32"

cd build.win32

echo "---------------------------------"
echo "----------cmake for vs2012-------"
echo "---------------------------------"
cmake -G"Visual Studio 11" -DCLIENT=true ../

popd
pause
