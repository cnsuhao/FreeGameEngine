
pushd ./ #1

cd `dirname $0`
cmd_path=`pwd`
root_path=$cmd_path/../

cd $root_path
pwd

echo "---------------------------------"
echo "-------generate lua script-------"
echo "---------------------------------"
python tool/toluaplus/tolua.py src/script/pkg/ src/script/luamid


cd $root_path
pwd

mkdir build.ios
cd build.ios

echo "---------------------------------"
echo "-------------cmake---------------"
echo "---------------------------------"
cmake -DCMAKE_TOOLCHAIN_FILE=../CMake/Toolchains/iOS.cmake -GXcode -DCLIENT=true ../

popd #1
