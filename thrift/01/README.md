the first demo to study thrift
================================

1. `thrift -r --gen cpp student.thrift`, will generate some files in the folder gen-cpp/
2. In the gen-cpp/ folder, you can use all of the files to build a server, I have modifed the file Serv\_server.skeleton.cpp to implement a simple\_server and nonblock\_server, and have the makefile done.
3. Also, there is a simple\_client and nonblock\_client implemented in the file client.cpp.

**test instructions:**

1. start the server
2. start the client
3. you will see the obj sent by the client on the server side, printed out in the screen
