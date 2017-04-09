#!/usr/bin/env python
# -*- coding:utf-8 -*-

# Author: carl
# Created on: 20170409
# History:

#! /bin/env python


'''
使用以下pb结构生成对象, 然后序列化, 然后写入文件

package mergefrom_trap;

message Person
{
    required string name = 1;
    required int32  age = 2;
    optional int32  new1 = 3;
    optional int32  new2 = 4[default = 0];
}

message AddressBook
{
    required string host_name = 1;
    repeated Person person = 2;
    optional int32  new3 = 3;
    optional int32  new4 = 4[default = 0];
}

'''


import sys
import mergefrom_trap_pb2



if __name__ == '__main__':
    if len(sys.argv) != 2:
        print "Usage:", sys.argv[0], "file_name"
        sys.exit(-1)
    filename = sys.argv[1]

    address_book = mergefrom_trap_pb2.AddressBook()

    address_book.host_name="carl"
    address_book.new3 = 3
    address_book.new4 = 4
    person = address_book.person.add()
    person.name='person1'
    person.age = 30
    person.new1 = 1
    person.new2 = 2

    print "address_book:", str(address_book)
    address_book_bin = address_book.SerializeToString()
    with open(filename, 'wb+') as f:
        f.write(address_book_bin)