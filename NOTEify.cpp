/*
****************************************
*NOTEify allows you to add new messages*
****************************************
*/
#include <fstream>
#include <list>
#include <algorithm>
#include <sstream>
#include "NOTE.h"

using namespace std;

void wait() {
  string temp;
  cout << "Please type enter to quit" << endl;
  getline(cin, temp);
}

void get_string(string& input_word, bool lower) {
  getline(cin, input_word);
  if (lower) {
    transform(input_word.begin(), input_word.end(), input_word.begin(), ::tolower);
  }
}

void print_help(string& input_word, const string& read_me) {
  cout << read_me << endl;
  get_string(input_word, true);
}

int main() {
  cout << "Loading notes..." << endl;

  //open the note name file
  list<Message> message_list;
  string title, message, input_line, input_word;
  int day, month, year;
  bool already_exists;

  ifstream note_file("messages.txt");
  if (note_file.good()) {
    //read in each line to parse
    while (getline(note_file, input_line)) { //while there exits lines to get
      stringstream linestream(input_line); //parse each line by word
      if (input_line != "") {
        linestream >> input_word;
      
        if (input_word == "-New_Message-") { //if the word in -New_Message-
          month = day = year = 0;
          title = message = "";
        }
        else if (input_word == "NAME:") { //if the word is NAME:
          while (linestream >> input_word) {
            title.append(input_word).append(" "); //read in each word in the name and add spaces accordingly
          }
          if (title.size() > 0) {
            title = title.substr(0, title.size() - 1); //remove the trailing space
          }
        }
        else if (input_word == "DATE:") { //if the word is DATE:
          linestream >> input_word;
          month = atoi(input_word.substr(0, 2).c_str()); //set the time variables based on the input
          day = atoi(input_word.substr(3, 5).c_str());
          year = atoi(input_word.substr(6, 10).c_str());
        }
        else if (input_word == "MESSAGE:") { //if the word is MESSAGE:
          while (linestream >> input_word) {
            message.append(input_word).append(" "); //read in each word in the the message and add spaces accordingly
          }
          if (message.size() > 0) {
            message = message.substr(0, message.size() - 1);
          }
        }
        else if (input_word == "END") { //if the word is END
          message_list.push_back(Message(title, Date(month, day, year), message)); //create our message and push it into our list of messages
        }
        else { //if the input_word is not recognized quit the program
          cerr << "ERROR: messages.txt could not be read properly. Aborting..." << endl << "problem seems to be word->" << input_word << endl;
          wait();
          return 0;
        }
      }
    }
  }
  else { //if the file could not be opened, quit
    cerr << "ERROR: messages.txt could not be opened correctly or does not exist. Aborting..." << endl;
    wait();
    return 0;
  }
  
  note_file.close();

  //parse commands until the user quits
  cout << "Notes are loaded correctly. Please NOTE ;) that changes will not be saved unless the quit command is used" << endl;

  while(true) {
    print_help(input_word, "Please enter a command.new note(n/new), delete note(d/delete), or quit(q/quit)");
    if (input_word == "n" || input_word == "new") {
      print_help(title, "Please enter a message name!"); //check that there is not already a message of that name
      list<Message>::iterator check_itr = message_list.begin();
      already_exists = false;
      for (;check_itr != message_list.end(); check_itr++) {
        if (check_itr->get_name() == title) {
          cout << "The name of your message is already in use. Please delete the other message or select a new name!";
          already_exists = true;
        }
      }
      if (already_exists == false) {
        print_help(message, "Please type your message!");
        print_help(input_word, "Please enter the month of the message!");
        month = atoi(input_word.c_str());
        print_help(input_word, "Please enter the day of the message!");
        day = atoi(input_word.c_str());
        print_help(input_word, "Please enter the year of the message!");
        year = atoi(input_word.c_str());
        message_list.push_back(Message(title, Date(month, day, year), message));
        cout << "Your message has been added!" << endl;
      }
    }
    else if (input_word == "d" || input_word == "delete") {
      print_help(input_word, "Please type the name of the message that you would like to erase!"); //**fix this
      list<Message>::iterator message_itr = message_list.begin();
      for (; message_itr != message_list.end(); message_itr++) {
        if (message_itr->get_name() == input_word) {
          message_list.erase(message_itr);
          break;
        }
      }
    }
    else if (input_word == "q" || input_word == "quit") {
      print_help(input_word, "Are you sure you want to quit?! (y or n)");
      //reopen the file and write all of the messages
      if (input_word == "y" || input_word == "yes") {
        ofstream new_file("messages.txt");
        if (new_file) {
          list<Message>::iterator message_itr = message_list.begin();
          for (; message_itr != message_list.end(); ++message_itr) {
            new_file << "-New_Message-\n" << "NAME: " << message_itr->get_name() << "\n" << "DATE: ";
            if (message_itr->get_month() < 10) {
              new_file << 0;
            }
            new_file << message_itr->get_month() << "/";
            if (message_itr->get_day() < 10) {
              new_file << 0;
            }
            new_file << message_itr->get_day() << "/" << message_itr->get_year() << "\n" << "MESSAGE: " << message_itr->get_message() << "\nEND\n\n";
          }
          new_file.close();
        }
        else {
          cout << "Error: messages.txt could not be written correctly. If this error persists, please check the file." << endl;
        }
        return 0;
      }
    }
    else {
      cout << "command " << input_word << " not recognized please use a valid word" << endl;
    }
  }

  return 0;
}
