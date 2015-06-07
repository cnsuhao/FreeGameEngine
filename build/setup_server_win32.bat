echo off

set cmd_path=%~dp0
set root_path=%cmd_path%..\

echo "root_path: %root_path%"

pushd %root_path%

echo "---------------------------------"
echo "-------generate lua script-------"
echo "---------------------------------"
python tool/toluaplus/tolua.py src/script/pkg/ src/script/luamid

if not exist %root_path%\buildserver.win32 mkdir %root_path%\buildserver.win32 

cd /d %root_path%\buildserver.win32 

cmake -G"Visual Studio 11" -DCLIENT=false ../

popd
pause