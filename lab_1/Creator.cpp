#include <iostream>
#include <fstream>
#include "Employee.h"
#include <set>


employee getValidEmployee();
int getValidNumber();
void getValidName(char* employeeName);
double getValidHours();

int main(int argc, char* argv[])
{

	std::ofstream file(argv[1]);
	int recordsNumber = atoi(argv[2]);

	employee Employee;
	for (int i = 1; i <= recordsNumber; i++) {
		std::cout << "Employee " << i << ": " << std::endl;

		Employee = getValidEmployee();

		file.write((char*) &Employee,sizeof(employee));

		std::cout << "\n";
	}

	file.close();
}
employee getValidEmployee() {

	employee Employee;

	Employee.num = getValidNumber();
	getValidName(Employee.name);
	Employee.hours = getValidHours();

	return Employee;
}

int getValidNumber() {
	double id_num;
	std::set <int> id;
	int position = -1;
	while (true) 
	{
		std::cout << "id number: ";
		if (std::cin >> id_num && id_num == (int)id_num )
		{
			if (id.find(id_num) == id.end()) {
				return id_num++;
			}
			return id_num;		
		}
		else 
		{
			std::cout << "Invalid Input! Please input an integer value." << std::endl;
			std::cin.clear();
			while (std::cin.get() != '\n');
		}
	}
}
void getValidName(char* employeeName)
{
	std::string name;
	int nameLength = 10;
	while (true) {
		std::cout << "name: ";
		std::cin >> name;
		if (name.length() <= nameLength) 
		{
			strcpy_s(employeeName,nameLength+1, name.c_str());
			return;
		}
		else {
			std::cout << "Invalid Input! Please input a value less or equal 10 chars." << std::endl;
		}

	}
}
double getValidHours() {
	double hours;
	while (true)
	{
		std::cout << "hours: ";
		if (std::cin >> hours)
		{
			return hours;
		}
		else
		{
			std::cout << "Invalid Input! Please input a double value." << std::endl;
			std::cin.clear();
			while (std::cin.get() != '\n');
		}
	}
}
