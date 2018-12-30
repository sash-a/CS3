#include "Database.h"
#include <sstream>  // Needed for file stream objects
#include <iostream> // Needed for console stream objects
#include <string>   // Needed for strings
#include <vector>   // Needed for the vector container
#include <fstream>

using namespace std;

vector<ABRSAS002::StudentRecord> ABRSAS002::Records;

void ABRSAS002::AddStudent(string name, \
	string surname, \
	string studentNumber, \
	string classRecord)
{
	cout << "Function AddStudent()\n\n";

	for (auto& record : ABRSAS002::Records)
	{
		if (record.StudentNumber == studentNumber)
		{
			record.Name = name;
			record.Surname = surname;
			record.ClassRecord = classRecord;
			return;
		}
	}
	ABRSAS002::StudentRecord sr = {name, surname, studentNumber, classRecord};
	ABRSAS002::Records.push_back(sr);
}

void ABRSAS002::ReadDb()
{
	cout << "Function readDb()\n\n";
	ifstream in("db.txt");

	if(!in)
		{ cout << "Couldn't open file" << endl; }

	// Remove all current inputs
	ABRSAS002::Records.clear();
	string line;
	while(getline(in, line))
	{
		// splitting string in file
		stringstream ss(line);
		vector <string> record;
		while(ss.good())
		{
			string substr;
			getline(ss, substr, '#');
			record.push_back(substr);
		}
		ABRSAS002::StudentRecord sr = {record[0], record[1], record[2], record[3]};
		ABRSAS002::Records.push_back(sr);
	}
	// Close file
	in.close();
}

void ABRSAS002::SaveDb()
{
  //save db
  cout << "Function saveDb()\n\n";

  ofstream outfile ("db.txt");
  for (auto& record: ABRSAS002::Records)
  {
  	string input = record.Name + "#" + record.Surname + "#" + record.StudentNumber + "#" + record.ClassRecord + "\n";
  	outfile << input;
  }
}

void ABRSAS002::GetData(string studentNumber)
{
  // get the student data
  cout << "Function getData()\n\n";
  for (auto& record : ABRSAS002::Records)
  {
  	if(record.StudentNumber == studentNumber)
  	{
  		cout << "Name: " << record.Name << endl;
  		cout << "Surname: " << record.Surname << endl;
  		cout << "Class record: " << record.ClassRecord << endl;
  		return;
  	}
  }
  cout << "nothing";
}

void ABRSAS002::Grade(string studentNumber)
{
	  // grade the student
	  cout << "Function grade()\n\n";
	  string classRecord;

	  // finding student
	  for (int i = 0; i < Records.size(); i++)
	  {
	  	if(ABRSAS002::Records[i].StudentNumber == studentNumber)
	  	{
	  		classRecord = ABRSAS002::Records[i].ClassRecord;
	  		break;
	  	}
	  }

	  double tot;
	  int count = 0;
	  // making the classRecord a stream
	  stringstream ss(classRecord);

	  while(ss.good())
	  {
	  	// Getting all info upto the ','
	    string substr;
	    getline(ss, substr, ',');
	    
	    // Making the string an int
	    stringstream toInt(substr);
	    int temp;
	    toInt >> temp;

	    // Adding to total and incrementing count
	    tot += temp;
	    count++;
	  }

	  // Getting average
	  tot /= count;
	  cout << tot << endl;
}