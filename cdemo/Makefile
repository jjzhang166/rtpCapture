all:
	rm -rf bin && mkdir bin
	gcc ./src/main.c -o ./bin/cdemo -L./../3rd-party/lib -lPocoUtil -lPocoNet -lPocoFoundation -lPocoXML -lPocoJSON -lRTPCaptureSDK -lrtmp -I./../3rd-party/include -Xlinker -rpath=./../3rd-party/lib
clean:
	rm -rf *.o
