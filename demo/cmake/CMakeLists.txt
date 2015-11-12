cmake_minimum_required(VERSION 2.6)
PROJECT(demo)

INCLUDE_DIRECTORIES($ENV{PWD}/../include)
LINK_DIRECTORIES($ENV{PWD}/../lib)
SET(EXECUTABLE_OUTPUT_PATH $ENV{PWD}/../bin)

ADD_EXECUTABLE(demo $ENV{PWD}/../src/main.cpp)

TARGET_LINK_LIBRARIES(demo pthread)
TARGET_LINK_LIBRARIES(demo libRTPCaptureSDK.a)
TARGET_LINK_LIBRARIES(demo libPocoUtild.a)
TARGET_LINK_LIBRARIES(demo libPocoNetd.a)
TARGET_LINK_LIBRARIES(demo libPocoFoundationd.a)
TARGET_LINK_LIBRARIES(demo srs_librtmp.a)

