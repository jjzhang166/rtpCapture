cmake_minimum_required(VERSION 2.6)
PROJECT(rtpSender)
ADD_DEFINITIONS(-DDEBUG)


INCLUDE_DIRECTORIES($ENV{PWD}/../../../3rd-party/include)
INCLUDE_DIRECTORIES($ENV{PWD}/../../../../../include)
LINK_DIRECTORIES($ENV{PWD}/../../../3rd-party/lib/)
LINK_DIRECTORIES($ENV{PWD}/../../../../../lib/pcap)


ADD_EXECUTABLE(rtpSender main.cpp)

TARGET_LINK_LIBRARIES(rtpSender PocoFoundationd)
TARGET_LINK_LIBRARIES(rtpSender PocoNetd)
TARGET_LINK_LIBRARIES(rtpSender pcap)

