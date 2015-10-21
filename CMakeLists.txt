cmake_minimum_required(VERSION 2.6)
PROJECT(rtpCapture)


INCLUDE_DIRECTORIES($ENV{PWD}/../../../include)
INCLUDE_DIRECTORIES($ENV{PWD}/../../../include/srs)
LINK_DIRECTORIES($ENV{PWD}/../../../lib)
SET(EXECUTABLE_OUTPUT_PATH ./release/bin)


ADD_EXECUTABLE(rtpCapture RTPHandler.cpp RTPDump.cpp CaptureCenter.cpp RTPCaptureSDK.cpp main.cpp)

TARGET_LINK_LIBRARIES(rtpCapture PocoFoundationd)
TARGET_LINK_LIBRARIES(rtpCapture PocoNetd)
#TARGET_LINK_LIBRARIES(rtpCapture $ENV{PWD}/../../lib/libPocoUtild.a)
#TARGET_LINK_LIBRARIES(rtpCapture $ENV{PWD}/../../lib/libPocoNetd.a)
#TARGET_LINK_LIBRARIES(rtpCapture $ENV{PWD}/../../lib/libPocoFoundationd.a)
TARGET_LINK_LIBRARIES(rtpCapture $ENV{PWD}/../../../lib/srs/srs_librtmp.a)

