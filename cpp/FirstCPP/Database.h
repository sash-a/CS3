#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

namespace ABRSAS002 {

	struct StudentRecord
	{
		std::string Name;
		std::string Surname;
		std::string StudentNumber;
		std::string ClassRecord;
	};

	extern std::vector<StudentRecord> Records;

	void AddStudent(std::string name, \
	std::string surname, \
	std::string studentNumber, \
	std::string classRecord);

	void ReadDb();

	void SaveDb();

	void GetData(std::string studentNumber);
	
	void Grade(std::string studentNumber);
}
#endif