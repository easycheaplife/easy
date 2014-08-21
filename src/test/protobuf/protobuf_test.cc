/*
  protobuf version:V2.5
  protobuf test.
  general:
	$export LD_LIBRARY_PATH=../../../dep/protobuf/src/.libs
	$../../../dep/protobuf/src/.libs/protoc -I../../../dep/protobuf/examples --cpp_out=. ../../../dep/protobuf/examples/addressbook.proto
  compile:
	$g++ -g -o protobuf_test addressbook.pb.h addressbook.pb.cc protobuf_test.cc -I../../../dep/protobuf/src/ -L../../../dep/protobuf/src/.libs -lprotobuf 
  run: 
    $./protobuf_test
*/
#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;

void PromptForAddress(tutorial::Person* __person) {
  int __id = 11120;
  __person->set_id(__id);

  __person->set_name("lee");

  string __email = "lijin157@163.com";
  __person->set_email(__email);

  string __number = "18510384228";
  tutorial::Person::PhoneNumber* __phone_number = __person->add_phone();
  __phone_number->set_number(__number);
  __phone_number->set_type(tutorial::Person::MOBILE);
}

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book) {
  for (int i = 0; i < address_book.person_size(); i++) {
    const tutorial::Person& person = address_book.person(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.has_email()) {
      cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phone_size(); j++) {
      const tutorial::Person::PhoneNumber& phone_number = person.phone(j);

      switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case tutorial::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case tutorial::Person::WORK:
          cout << "  Work phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
  }
}

// 	 Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  tutorial::AddressBook __address_book;

  // Add an address.
  PromptForAddress(__address_book.add_person());
  string __string;
  // Serialize to string
  if (!__address_book.SerializeToString(&__string)) 
  {
	cerr << "Failed to write address book." << endl;
    return -1;
  }
  // parse from string
  __address_book.ParseFromString(__string);
  ListPeople(__address_book);
  
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}