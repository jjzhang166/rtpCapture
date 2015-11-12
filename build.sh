#/bin/bash

echo $PWD

# build sdk
cd sdk
rm -rf build
mkdir build
cd build
cmake ..
make

# copy lib include
cd ../..
cp -rf ./src/RTPCaptureSDK.h ./demo/include/
cp -rf ./../../lib/poco/libPocoFoundationd.a demo/lib/
cp -rf ./../../lib/poco/libPocoNetd.a demo/lib/
cp -rf ./../../lib/poco/libPocoUtild.a demo/lib/
cp -rf ./../../lib/srs/libsrs_librtmp.a demo/lib/

# build demo
cd demo
rm -rf bin && mkdir bin
rm -rf log && mkdir log
rm -rf movie && mkdir movie

cd cmake
cmake .
make

# run demo
cd ..
./bin/demo
