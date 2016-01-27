/* Hospital Management.cpp : Defines the entry point for the console application.

Author: Stoyan Dimitrov
Date:	01.09.2016

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <map>
using namespace std;

class Calendar {															// the calendar class
public:
	string name;
	string egn;
	string complaint;
	string dateOfAppointment;
	string DateOfComplaints;
	string diagnose;
	string drugsPrescribed;

	Calendar::Calendar(){}
	Calendar::Calendar(string, string, string, string, string, string, string);
};

Calendar::Calendar(string newName, string newEgn, string newComplaint, string newDateOfAppointment,
	string newDateOfComplaints, string newDiagnose, string newDrugsPrescribed) {
	name = newName;
	egn = newEgn;
	complaint = newComplaint;
	dateOfAppointment = newDateOfAppointment;
	DateOfComplaints = newDateOfComplaints;
	diagnose = newDiagnose;
	drugsPrescribed = newDrugsPrescribed;
};

class Date {																// the date class
public:
	Calendar* patient;
	Date::Date(Calendar*);
	Date::Date();
	bool remove(Calendar *c);
	void getLastMonthPatients(string date);
	void getMostCommonDiagnose();
	void displayDate(Date*);
	void displayAllDates();
	void getOnlyComplaints();
	void insert(Date*);
	bool isComplaint;
	friend ostream& operator<< (ostream &out, Date* member);
	Date *next;
};

Date::Date(Calendar *cal) {													// date constructor with parameter calendar for setting date
	patient = cal;
};

Date::Date(){}																// date empty constructor
Date* first;
FILE* file;

void Date::insert(Date* newPatient) {										// create files
	// Create a new Date 
	newPatient->next = NULL;
	// Create a temp pointer
	Date *tmp = first;

	if (tmp != NULL) {
		file = fopen("Complaints.bin", "wb");
		fwrite(&tmp, sizeof(tmp), 1, file);
		fclose(file);
		// Nodes already present in the Date
		// Parse to end of Date
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		// Point the last node to the new node
		tmp->next = newPatient;
	}
	else  {
		// First node in the Date
		first = newPatient;
		file = fopen("Items.bin", "wb");
		fwrite(&tmp, sizeof(tmp), 1, file);
		fclose(file);
	}
	//fclose(file);
}

void Date::displayDate(Date *node) {										// display the date using the overrider operator '<<'

	cout << node; //Using the overridden operator - <<
}

//Type in data 
//*******************************************************
//    CASE       : 1
//    DETAILS    : ADD A PATIENT RECORD
//*******************************************************
Date* getData() {															// create a Patient
	string name;
	string egn;
	string complaint;
	string dateOfAppointment;
	string DateOfComplaints;
	string diagnose;
	string drugsPrescribed;
	Date *data;

	string hasComplain;

	cout << "Enter data: " << endl;
	cout << "Name: "; cin >> name;
	cout << "Egn: "; cin >> egn;

	cout << "Has complain? <yes or no>: ";

	while (hasComplain != "yes" && hasComplain != "no") {
		cin >> hasComplain;
	}

	if (hasComplain == "yes") {
		cout << "Complaint: "; cin >> complaint;
	}

	cout << "Date of appointment: "; cin >> dateOfAppointment;
	cout << "Diagnose: "; cin >> diagnose;
	cout << "Drugs prescribed: "; cin >> drugsPrescribed;
	cout << "Date of complaints: "; cin >> DateOfComplaints;
	cout << "\n";

	data = new Date(new Calendar(name, egn, complaint, dateOfAppointment, DateOfComplaints, diagnose, drugsPrescribed)); //Creates an object which represents a component
	cin.clear();
	cin.ignore(200, '\n');
	return data;
}

//*******************************************************
//    CASE       : 2
//    DETAILS    : SHOW ALL PATIENT RECORDS
//*******************************************************
void Date::displayAllDates(){												
	Date *pCur = first;
	int nodeCount = 1;
	while (pCur) {
		cout << "Patient " << nodeCount++ << ": \n";
		displayDate(pCur);
		cout << endl;
		pCur = pCur->next;
	}
}

//*******************************************************
//    CASE       : 3
//    DETAILS    : DELETE A PATIENT BY EGN
//*******************************************************
bool Date::remove(Calendar *c){												// iztrivane na data
	if (first == 0) {
		cout << "Empty list!" << endl;
	}
	else {
		Date *pPre = NULL;
		Date *pCur = first;

		while (pCur != NULL)  {
			if (pCur->patient == c)   {
				break;
			}
			else   {
				pPre = pCur;
				pCur = pCur->next;
			}
		}
		if (pCur == NULL)  {
			cout << "Current date cannot be deleted." << endl;
		}
		else  {
			if (first == pCur)   {
				first = first->next;
			}
			else   {
				pPre->next = pCur->next;
			}
			delete pCur;
			cout << " DATE REMOVED\n" << endl;
			return true;
		}
	}
	return false;
}

//*******************************************************
//    CASE       : 5
//    DETAILS    : LAST MONTH PATIENTS
//*******************************************************
void Date::getLastMonthPatients(string date)								// vzimane na pacientite za posledniq mesec
{
	Date *pCur = first;
	int nodeCount = 1;

	while (pCur != NULL) {
		if (date == (pCur->patient->dateOfAppointment))  {
			cout << endl;
			cout << pCur;
		}
			pCur = pCur->next;
	}
}

//*******************************************************
//    CASE       : 6
//    DETAILS    : MOST COMMON DIAGNOSE
//*******************************************************
void Date::getMostCommonDiagnose()											// vzemane na nai - povtarqshtata se diagnoza
{
	std::map<string, int> diagnoseCounter;

	string diagnose;
	Date *pCur = first;

	string mostCommonDiagnose;
	int mostCommonDiagnoseCounter = 0;

	while (pCur != NULL) {
		diagnose = pCur->patient->diagnose;

		if (diagnoseCounter.find(diagnose) == diagnoseCounter.end()) {
			diagnoseCounter[diagnose] = 1;
		}
		else {
			diagnoseCounter[diagnose]++;
		}

		if (diagnoseCounter[diagnose] > mostCommonDiagnoseCounter) {
			mostCommonDiagnose = diagnose;
			mostCommonDiagnoseCounter = diagnoseCounter[diagnose];
		}

		pCur = pCur->next;
	}

	cout << "Most common diagnose: \n" << mostCommonDiagnose << endl;
}

void Date::getOnlyComplaints()												// vzimane na pacientite s oplakvaniq
{
	Date *pCur = first;

	string complaint;

	while (pCur != NULL) {
		complaint = pCur->patient->complaint;

		if (complaint.length() > 0) {
			displayDate(pCur);
		}

		pCur = pCur->next;
	}
}

ostream& operator<<(ostream& out, Date* pA){								// predefinirane na operatora za izvejdane '<<'
	out << "\n";
	out << "Name:" << pA->patient->name << "\n";
	out << "EGN:" << pA->patient->egn << "\n";
	out << "Complaint:" << pA->patient->complaint << "\n";
	out << "Date of appointment:" << pA->patient->dateOfAppointment << "\n";
	out << "Diagnose:" << pA->patient->diagnose << "\n";
	out << "Drugs prescribed:" << pA->patient->drugsPrescribed << "\n";
	out << "Date of complaints:" << pA->patient->DateOfComplaints << "\n";
	out << "\n";
	return out;
}

Calendar* getDate(string ID){												// vzimane na datata
	Date *pCur = first;
	int nodeCount = 1;

	while (pCur != NULL) {
		if (ID == (pCur->patient->egn))  {
			cout << endl;
			return pCur->patient;
		}
		else
			pCur = pCur->next;
	}

	return NULL;
}

//*******************************************************
//						MENU
//*******************************************************
void printMenu(){
	cout << "1.Insert a patient.\n";
	cout << "2.Print all information about patients.\n";
	cout << "3.Delete a patient by EGN.\n";
	cout << "4.Patients with complaints.\n";
	cout << "5.Last month patients.\n";
	cout << "6.Most common diagnose.\n\n";
}

//*************************************************************
//						Main Function
//**************************************************************
int main(){
	Date ll;
	Date *pA = NULL;
	Calendar* c;
	string egn;
	string date;
	while (true) {
		printMenu();
		int opt;
		cin >> opt;
		switch (opt)
		{
		case 1:
		{
				  ll.insert(getData());
				  break;
		}
		case 2:
		{
				  ll.displayAllDates();
				  break;
		}
		case 3:
		{
				  cout << "Enter patient egn: ";
				  cin >> egn;
				  c = getDate(egn);
				  ll.remove(c);
				  break;
		}
		case 4:
		{
				  ll.getOnlyComplaints();
				  break;
		}
		case 5:
		{
				  cout << "Enter date of appointment: ";
				  cin >> date;
				  ll.getLastMonthPatients(date);
				  break;
		}
		case 6:
		{
				  ll.getMostCommonDiagnose();
				  break;
		}

		default:
		{
				   cout << "Invalid input!" << endl;
		}exit(0);
		}
	}
	return 0;
}