#/bin/bash

echo aa

# build sdk
cd sdk
#rm -rf build
#mkdir build
cd build
cmake ..
make

cd ../..
cp -rf ./src/RTPCaptureSDK.h ./demo/include/

cd demo/cmake
#rm -rf build
#mkdir build
cmake .
make

cd ../../..
cd demo
./bin/demo
