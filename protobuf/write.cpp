#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"


using namespace std;


// this function fills in a Person message based on user input.
void prompt_for_address(Person *person)
{
    cout << "enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "enter name: ";
    getline(cin, *person->mutable_name());

    cout << "enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty())
    {
        person->set_email(email);
    }

    while (true)
    {
        cout << "enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty())
        {
            break;
        }
        Person::PhoneNumber *phone_number = person->add_phone();
        phone_number->set_number(number);

        cout << "is this a mobile, home, or work phone?";
        string type;
        getline(cin, type);
        if ("mobile" == type)
        {
            phone_number->set_type(Person::MOBILE);
        }
        else if ("home" == type)
        {
            phone_number->set_type(Person::HOME);
        }
        else if ("work" == type)
        {
            phone_number->set_type(Person::WORK);
        }
        else
        {
            cout << "unknown phone type. using default." << endl;
        }
    }
    return;
}

// main function: reads the entire address book from a file,
// add one person based on user input, then writes it back to 
// the same file
int main(int argc, char **argv)
{
    // verify that the version of the library that we linked against is 
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " address_book_file" << endl;
        return -1;
    }

    AddressBook address_book;

    // read the existing address book
    fstream input(argv[1], ios::in | ios::binary);
    if (!input)
    {
        cout << argv[1] << ": File not found. Creating a new file." << endl;
    }
    else if (!address_book.ParseFromIstream(&input))
    {
        cerr << "failed to parse address book." << endl;
        return -1;
    }

    // add an addres
    prompt_for_address(address_book.add_person());

    // write the new address back to disk
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output))
    {
        cerr << "failed to write address book." << endl;
        return -1;
    }

    // optional: delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
