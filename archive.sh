#/bin/bash

#rm -rf demo/cmake/CMakeCache.txt
#rm -rf demo/cmake/CMakeFiles
#rm -rf demo/cmake/cmake_install.cmake
#rm -rf demo/cmake/Makefile
#rm -rf demo/log/*
#rm -rf demo/movie/*

#tar -cjf demo.tar.bz2 demo

rm -rf cdemo/log/*
rm -rf cdemo/movie/*
tar -cjf cdemo.tar.bz2 cdemo
