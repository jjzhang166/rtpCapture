cmake_minimum_required(VERSION 2.6)
PROJECT(rtpCapture)


INCLUDE_DIRECTORIES($ENV{PWD}/../../../include)
INCLUDE_DIRECTORIES($ENV{PWD}/../../../include/srs)
LINK_DIRECTORIES($ENV{PWD}/../../../lib/poco)
LINK_DIRECTORIES($ENV{PWD}/../../../lib/srs)
SET(EXECUTABLE_OUTPUT_PATH ./release/bin)

ADD_EXECUTABLE(rtpCapture RTPHandler.cpp RTPDump.cpp CaptureCenter.cpp RTPCaptureSDK.cpp main.cpp)

TARGET_LINK_LIBRARIES(rtpCapture pthread)
TARGET_LINK_LIBRARIES(rtpCapture libPocoUtild.a)
TARGET_LINK_LIBRARIES(rtpCapture libPocoNetd.a)
TARGET_LINK_LIBRARIES(rtpCapture libPocoFoundationd.a)
TARGET_LINK_LIBRARIES(rtpCapture srs_librtmp.a)

