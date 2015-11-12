cmake_minimum_required(VERSION 2.6)
PROJECT(RTPCaptureSDK)

INCLUDE_DIRECTORIES($ENV{PWD}/../../../../include)
INCLUDE_DIRECTORIES($ENV{PWD}/../../../../include/srs)
set(LIBRARY_OUTPUT_PATH $ENV{PWD}/../../demo/lib)

ADD_LIBRARY(RTPCaptureSDK ../../src/RTPHandler.cpp 
                          ../../src/RTPDump.cpp 
                          ../../src/CaptureCenter.cpp 
                          ../../src/ReceiverCenter.cpp 
                          ../../src/RTPCaptureSDK.cpp)

