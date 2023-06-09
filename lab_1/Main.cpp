﻿#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "../Creator/Employee.h"


void launchCreator(std::string fileNameBin, int recordsNumber);
void launchReporter(std::string fileNameBin, std::string fileNameReport, double hourlyWage);
void launchProcess(std::string Args);
void showReportInfo(const std::string fileNameReport);
void showBinaryFileInfo(const std::string fileNameEmployees);

int main(int argc, char* argv[])
{


	std::string fileNameBin;
	int recordsNumber;
	std::cout << "Enter binary file name: ";
	std::cin >> fileNameBin;
	std::cout << "Enter the number of records: ";
	std::cin >> recordsNumber;

	launchCreator(fileNameBin,recordsNumber);
	showBinaryFileInfo(fileNameBin);

	std::string fileNameReport;
	double hourlyWage;
	std::cout << "Enter report file name: ";
	std::cin >> fileNameReport;
	std::cout << "Enter the salary_per_hour: ";
	std::cin >> hourlyWage;

	launchReporter(fileNameBin, fileNameReport, hourlyWage);
	showReportInfo(fileNameReport);

}
void launchCreator(std::string fileNameBin, int recordsNumber)
{
	std::string Args = "Creator.exe "  + fileNameBin + " " + std::to_string(recordsNumber);
	launchProcess(Args);
}

void launchReporter(std::string fileNameBin, std::string fileNameReport, double hourlyWage)
{
	std::string Args = "Reporter.exe " + fileNameBin + " "  + fileNameReport + " " + std::to_string(hourlyWage);
	launchProcess(Args);
}
void launchProcess(const std::string Args)
{
	LPSTR comArgs = const_cast<char*>(Args.c_str());

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	CreateProcess(nullptr, comArgs, nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
void showReportInfo(const std::string fileNameReport)
{
	std::ifstream fileReport(fileNameReport);
	if (!fileReport) {
		std::cout << "Error occured while opening the file " << fileNameReport << std::endl;
		return;
	}

	std::string line;
	while (getline(fileReport, line)) {
		std::cout << line << std::endl;
	}

}
void showBinaryFileInfo(const std::string fileNameEmployees)
{
	std::ifstream fileEmployees(fileNameEmployees, std::ios::binary);

	if (!fileEmployees) {
		std::cout << "Error occured while opening the file " << fileNameEmployees << std::endl;
		return;
	}

	employee Employee;
	while (fileEmployees.read((char*)&Employee, sizeof(employee)))
	{
		std::cout << std::left << std::setw(17) << Employee.num << std::setw(15) << Employee.name << std::setw(7) <<
			Employee.hours << std::setw(6) << std::endl;
	}

	fileEmployees.close();
}
