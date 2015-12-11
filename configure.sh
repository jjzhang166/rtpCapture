#/bin/sh

workspace=${PWD}

#-------------------------------------------------------------------
echo  "install poco lib"
cd $workspace/3rd-party/poco-1.6.1
./configure --prefix=.. --no-samples --no-tests
make && make install

#-------------------------------------------------------------------
echo "install librtmp"
cd $workspace/3rd-party/librtmp
make
rm -rf $workspace/3rd-party/include/srs
mkdir $workspace/3rd-party/include/srs
cp -rf objs/include/srs_librtmp.h $workspace/3rd-party/include/srs/
cp -rf objs/lib/librtmp.so $workspace/3rd-party/lib

#-------------------------------------------------------------------
echo "install sdk"
cd $workspace/sdk
if [ ! -d "build" ];then
mkdir build
fi
cd build
cmake ..
make
cp -rf $workspace/src/RTPCapture.h $workspace/3rd-party/include

#-------------------------------------------------------------------
echo "install cdemo"
cd $workspace/cdemo
make

