#/bin/bash

echo $PWD

# build sdk
cd sdk

if [ ! -d "build" ];then
mkdir build
fi
cd build
cmake ..
make

# copy lib include
#cd ../..
#cp -rf ./src/RTPCaptureSDK.h ./demo/include/
#cp -rf ./../../lib/poco/libPocoFoundationd.a demo/lib/
#cp -rf ./../../lib/poco/libPocoNetd.a demo/lib/
#cp -rf ./../../lib/poco/libPocoUtild.a demo/lib/
#cp -rf ./../../lib/srs/libsrs_librtmp.a demo/lib/

# build demo
#cd demo
#rm -rf bin && mkdir bin
#rm -rf log && mkdir log
#rm -rf movie && mkdir movie

#cd cmake
#cmake .
#make
cd ../..

# run demo
#cd ..
#./bin/demo

#copy lib include
cp -rf ./src/RTPCapture.h ./cdemo/include/
cp -rf ../../lib/srs/librtmp.so                 ./cdemo/lib/        
cp -rf ../../lib/poco/libPocoUtil.so.31         ./cdemo/lib/        
cp -rf ../../lib/poco/libPocoNet.so.31          ./cdemo/lib/
cp -rf ../../lib/poco/libPocoFoundation.so.31   ./cdemo/lib/
cp -rf ../../lib/poco/libPocoXML.so.31          ./cdemo/lib/
cp -rf ../../lib/poco/libPocoJSON.so.31         ./cdemo/lib/
cd cdemo/lib
ln -sf ./libPocoUtil.so.31            ./libPocoUtil.so
ln -sf ./libPocoNet.so.31             ./libPocoNet.so
ln -sf ./libPocoFoundation.so.31      ./libPocoFoundation.so
ln -sf ./libPocoXML.so.31             ./libPocoXML.so
ln -sf ./libPocoJSON.so.31            ./libPocoJSON.so
cd ../../

cd cdemo
if [ ! -d "bin" ];then
mkdir bin
fi

if [ ! -d "movie" ];then
mkdir movie
fi

if [ ! -d "log" ];then
mkdir log
fi

make
