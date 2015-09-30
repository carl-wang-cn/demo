#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"


using namespace std;

void list_people(const AddressBook &address_book)
{
    for (int i=0; i<address_book.person_size(); ++i)
    {
        const Person &person = address_book.person(i);

        cout << "Person ID: " << person.id() << endl;
        cout << "    Name:  " << person.name() << endl;
        if (person.has_email()) // true only if person.set_email() was called before
        {
            cout << "   Email: " << person.email() << endl;
        }

        for (int j = 0; j < person.phone_size(); ++j)
        {
            const Person::PhoneNumber &phone_number = person.phone(j);

            switch (phone_number.type())
            {
                case Person::MOBILE:
                    cout << "Mobile phone #: ";
                    break;
                    
                case Person::HOME:
                    cout << "Home phone #: ";
                    break;

                case Person::WORK:
                    cout << "Work phone #: ";
                    break;
            }
            cout << phone_number.number() << endl;
        }
    }
}

// main function: reads the entire address book from a file and prints all
// the information inside
int main(int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2)
    {
        cerr << "usage:  " << argv[0] << " address_book_file" << endl;
        return -1;
    }

    AddressBook address_book;

    // read the existing address book
    fstream input(argv[1], ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input))
    {
        cerr << "failed to parse address book." << endl;
        return -1;
    }

    // protobuf can print out the structed message
    cout << address_book.DebugString() << endl; 

    list_people(address_book);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

