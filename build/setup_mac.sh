
pushd `dirname $0`

cmd_path=`pwd`
root_path=$cmd_path/../

cd $root_path
pwd

if [ ! -d build.mac ]
then
	mkdir build.mac
fi

cd build.mac

echo "---------------------------------"
echo "-------------cmake---------------"
echo "---------------------------------"
cmake -GXcode ../

popd #1
