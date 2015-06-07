@echo off

pushd %~dp0..\

echo "---------------------------------"
echo "-------generate lua script-------"
echo "---------------------------------"
python tool/tolua++/tolua.py src/script/pkg/ src/script/luamid

popd
