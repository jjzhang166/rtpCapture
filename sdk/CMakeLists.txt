cmake_minimum_required(VERSION 2.6)
PROJECT(RTPCaptureSDK)

INCLUDE_DIRECTORIES($ENV{PWD}/../../../../include)
INCLUDE_DIRECTORIES($ENV{PWD}/../../../../include/srs)
set(LIBRARY_OUTPUT_PATH $ENV{PWD}/../../cdemo/lib)

add_definitions("-fPIC")

ADD_LIBRARY(RTPCaptureSDK SHARED ../../src/RTPHandler.cpp 
                          ../../src/RTPDump.cpp 
                          ../../src/CaptureCenter.cpp 
                          ../../src/ReceiverCenter.cpp 
                          ../../src/RTPCaptureSDK.cpp
                          ../../src/RTPCapture.cpp)
