simple_client:
	g++ -g -I/usr/local/include/thrift -I gen-cpp/ gen-cpp/Serv.cpp gen-cpp/student_types.cpp gen-cpp/student_constants.cpp client.cpp -o client -lthrift -DSIMPLE_SERVER

nonblock_client:
	g++ -g -I/usr/local/include/thrift -I gen-cpp/ gen-cpp/Serv.cpp gen-cpp/student_types.cpp gen-cpp/student_constants.cpp client.cpp -o client -lthrift -DNONBLOCK_SERVER

clean:
	rm client -f
