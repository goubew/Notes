/*
*******************************************************************
*NOTEifications checks for error messages and deletes old messages*
*******************************************************************
*/
#include <fstream>
#include <ctime>
#include <list>
#include <stdlib.h>
#include <sstream>
#include "NOTE.h"

using namespace std;

void wait() {
  string temp;
  cout << "Type enter to quit"<< endl;
  getline(cin, temp);
}

int main() {
  cout << "Loading notes..." << endl;

  //open the note name file
  list<Message> message_list;
  string title, message, input_line, input_word;
  int day, month, year;

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
    
  //check if message_list is empty, if so, we don't need the current date
  if (message_list.size() > 0) {
    //list the number of messages read
    cout << message_list.size() << " messages were read.\n" << endl;
    //get the current date
    int current_day, current_month, current_year;

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    current_day = timeinfo->tm_mday;
    current_month = timeinfo->tm_mon + 1;
    current_year = timeinfo->tm_year + 1900;

    cout << "the current date is->" << current_month << "/" << current_day << "/" << current_year << endl; // delete this line
    list<Message>::iterator message_itr = message_list.begin();
    for (; message_itr != message_list.end(); ++message_itr) {
      cout << message_itr->get_month() << "/" << message_itr->get_day() << "/" << message_itr->get_year() << endl;
    }

    Date today(current_month, current_day, current_year);
    
    //check if the messages are set to be displayed today
    message_itr = message_list.begin();
    for (; message_itr != message_list.end(); ++message_itr) {
      if (message_itr->get_date() == today) {
	      //print the messages that are necessary
	      cout << "*" << message_itr->get_name() << "*" << endl;
	      cout << "Date: " << message_itr->get_month() << "/" << message_itr->get_day() << "/" << message_itr->get_year() << endl;
	      cout << message_itr->get_message() << endl << endl;
      }
    }

    //remake the messages.txt file removing any messages that are old
    ofstream new_file("messages.txt");
    if (new_file) {
      list<Message>::iterator message_itr = message_list.begin();
      for (; message_itr != message_list.end(); ++message_itr) {
	      if (today < message_itr->get_date() || today == message_itr->get_date()) {
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
      }
      new_file.close();
    }
    else { //if the new file could not be made, quit
      cerr << "ERROR: messages.txt could not be written correctly. If this error persists, please check the file." << endl;
      wait();
      return 0;
    }
  }
  else { //if no messages can be displayed, quit
    cout << "You have no messages to display please use NOTEify.exe to add more." << endl;
    wait();
    return 0;
  }
  
  //ending stuff
  cout << "Type Enter to quit" << endl;
  getline(cin, input_word);
  return 0;
}