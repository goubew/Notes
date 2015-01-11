#include <iostream>
#include <string>
using namespace std;

class Date {
public:
  //constructors
  Date(int month, int day, int year): month_(month), day_(day), year_(year) {}

  //variables
  int month_;
  int day_;
  int year_;
};

class Message {
public:
  //constructors
  Message(string name, Date date, string message): name_(name), date_(date), message_(message)  {}

  //methods
  const string& get_name() const { return name_; }
  const Date& get_date() const { return date_; }
  const string& get_message() const { return message_; }
  int get_month(){ return date_.month_; }
  int get_day() { return date_.day_; }
  int get_year() { return date_.year_; }
  
private:
  //variables
  string name_;
  Date date_;
  string message_;
};

inline bool operator< (const Date& lhs, const Date& rhs) {
  if (lhs.year_ < rhs.year_) { return true; }
  else if (lhs.year_ == rhs.year_) {
    if (lhs.month_ < rhs.month_) { return true; }
    else if (lhs.month_ == rhs.month_) {
      if (lhs.day_ < rhs.day_) { return true; }
    }
  }
  return false;
}

inline bool operator== (const Date& lhs, const Date& rhs) {
  return (lhs < rhs == false && rhs < lhs == false);
}
