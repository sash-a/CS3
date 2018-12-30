#include <iostream>
#include <cstdlib>
#include "Database.h"

using namespace std;

void askInput()
{
  std::cout << "\
  0: Add student\n\
  1: Read database\n\
  2: Save database\n\
  3: Display student data\n\
  4: Grade student\n\
  q: Quit\n\
  Enter a number (or q to quit) and press return...\n"; 
}


int main(void)
{
  for(;;){
    askInput();

    std::string answer;
    std::cin >> answer;
    system("clear");

    // determine which method to call
    if (answer == "q") break;
    else if (answer == "0") 
    {
      string name;
      string surname;
      string studentNumber;
      string classRecord;

      cout << "Enter username" << endl;
      cin >> name;

      cout << "Enter surname" << endl;
      cin >> surname;

      cout << "Enter studentNumber" << endl;
      cin >> studentNumber;

      cout << "Enter class records separate by ','" << endl;
      cin >> classRecord;
      cout << "\n";
      ABRSAS002::AddStudent(name, surname, studentNumber, classRecord);
    }
    else if (answer == "1") ABRSAS002::ReadDb();
    else if (answer == "2") ABRSAS002::SaveDb();
    else if (answer == "3") 
    { 
        string studentNo;
        cout << "Enter student number: ";
        cin >> studentNo;
        ABRSAS002::GetData(studentNo);
    }
    else if (answer == "4") 
    {
      string studentNo;
      cout << "Enter student number: ";
      cin >> studentNo; 
      ABRSAS002::Grade(studentNo);
    }
  }
  return 0;
}