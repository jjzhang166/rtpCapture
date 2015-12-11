#/bin/bash

rm -rf mkdir libRTPCapture
mkdir libRTPCapture
mkdir libRTPCapture/3rd-party
mkdir libRTPCapture/3rd-party/include
mkdir libRTPCapture/3rd-party/lib
mkdir libRTPCapture/cdemo
mkdir libRTPCapture/cdemo/log
mkdir libRTPCapture/cdemo/movie

cp -rf cdemo/src      ./libRTPCapture/cdemo
cp -rf cdemo/Makefile ./libRTPCapture/cdemo

cp -rf 3rd-party/include/RTPCapture.h          ./libRTPCapture/3rd-party/include
cp -rf 3rd-party/lib/librtmp.so                ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libRTPCaptureSDK.so       ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libPocoUtil.so.31         ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libPocoNet.so.31          ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libPocoFoundation.so.31   ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libPocoXML.so.31          ./libRTPCapture/3rd-party/lib/
cp -rf 3rd-party/lib/libPocoJSON.so.31         ./libRTPCapture/3rd-party/lib/
cd libRTPCapture/3rd-party/lib
ln -sf ./libPocoUtil.so.31            ./libPocoUtil.so
ln -sf ./libPocoNet.so.31             ./libPocoNet.so
ln -sf ./libPocoFoundation.so.31      ./libPocoFoundation.so
ln -sf ./libPocoXML.so.31             ./libPocoXML.so
ln -sf ./libPocoJSON.so.31            ./libPocoJSON.so


cd ../../..
tar -cjf libRTPCapture.tar.bz2 ./libRTPCapture
rm -rf libRTPCapture
