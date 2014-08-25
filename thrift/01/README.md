the first demo to study thrift
================================

1. `thrift -r --gen cpp student.thrift`, this will generate some files in the folder gen-cpp/
2. in the gen-cpp/ folder, you can use all of the files to build a server, I have modifed the file Serv\_server.skeleton.cpp to imply a simple\_server and nonblock\_server, and have the makefile done.
3. also, there is also a simple\_client and nonblock\_client implemented in the file client.cpp.

**ps:**
start the server, then start the client, you will see the obj sent by the client on the server side, printed out in the screen
