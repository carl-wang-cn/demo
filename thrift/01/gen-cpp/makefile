simple_server:
	g++ -g -I /usr/local/include/thrift/ *.cpp -o server -lthrift -DSIMPLE_SERVER

nonblock_server:
	g++ -g -I /usr/local/include/thrift/ *.cpp -o server -lthriftnb -levent -lthrift  -DNONBLOCK_SERVER

clean:
	rm server -f
