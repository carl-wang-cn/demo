/*
 * Author: carl
 * Created on: 20170409
 * History:
 * */

#include <iostream>
#include <fstream>
#include <string>

#include <hiredis/hiredis.h>
#include "mergefrom_trap.pb.h"

using namespace mergefrom_trap;
using namespace std;

void read_write_read_with_copyfrom(const string &filename, AddressBook &book)
{
    {
        fstream read_f(filename.c_str(), ios::in | ios::binary);
        if (!read_f)
        {
            cerr << filename << ": file not found, exit..." << endl;
            return;
        }

        book.ParseFromIstream(&read_f);
        cout << "book: " << book.DebugString() << endl;
    }

    book.set_host_name("carl_copyfrom_update");
    int index = 0;
    for (; index < book.person_size(); ++index)
    {
        Person person;
        person.CopyFrom(book.person(index));
        person.set_age(100);

        book.mutable_person(index)->CopyFrom(person);
    }

    {
        fstream write_f(filename.c_str(), ios::out | ios::trunc | ios::binary);
        if (!book.SerializeToOstream(&write_f))
        {
            cerr << "failed to write addressbook" << endl;
            return;
        }
    }

    {
        fstream read_f(filename.c_str(), ios::in | ios::binary);
        if (!read_f)
        {
            cerr << filename << ": file not found, exit..." << endl;
            return;
        }

        book.ParseFromIstream(&read_f);
        cout << "book: " << book.DebugString() << endl;
    }

}


void read_write_read_with_mergefrom(const string &filename, AddressBook &book)
{
    {
        fstream read_f(filename.c_str(), ios::in | ios::binary);
        if (!read_f)
        {
            cerr << filename << ": file not found, exit..." << endl;
            return;
        }

        book.ParseFromIstream(&read_f);
        cout << "book: " << book.DebugString() << endl;
    }

    book.set_host_name("carl_update_mergefrom");
    int index = 0;
    for (; index < book.person_size(); ++index)
    {
        Person person;
        person.CopyFrom(book.person(index));
        person.set_age(100);

        book.mutable_person(index)->MergeFrom(person);
    }

    {
        fstream write_f(filename.c_str(), ios::out | ios::trunc | ios::binary);
        if (!book.SerializeToOstream(&write_f))
        {
            cerr << "failed to write addressbook" << endl;
            return;
        }
    }

    {
        fstream read_f(filename.c_str(), ios::in | ios::binary);
        if (!read_f)
        {
            cerr << filename << ": file not found, exit..." << endl;
            return;
        }

        book.ParseFromIstream(&read_f);
        cout << "book: " << book.DebugString() << endl;
    }

}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
        return -1;
    }

    AddressBook book;
    string filename(argv[1]);

    for (int i = 0; i < 3; ++i)
    {
        read_write_read_with_copyfrom(filename, book);
    }

    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;
    cout << "a big trap is coming ...." << endl;

    for (int i = 0; i < 3; ++i)
    {
        read_write_read_with_mergefrom(filename, book);
    }

    return 0;
}