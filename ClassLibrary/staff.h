#pragma once

#include "DOB.h"

public class Staff
{

public:
	int id = 0;
	std::string firstName = "";
	std::string lastName = "";
	std::string commonName = "";
	std::string fullName = ""; 
	std::string club = "";
	DOB dob;

	Staff() {
	}

	Staff(int index, std::string firstName, std::string lastName, std::string commonName, std::string dobString, std::string club)
	{
		id = index;
		this->firstName = firstName;
		this->lastName = lastName;
		this->commonName = commonName;
		fullName = firstName + " " + lastName;					
		this->club = club;
		dob = DOB(dobString);
	}	

	static bool Compare(Staff s1, Staff s2)
	{
		if (DOB::Compare(s1.dob, s2.dob) && s1.fullName.compare(s2.fullName) == 0 && s1.club.compare(s2.club) == 0)
		{
			return true;
		}
		return false;
	}

	std::string print()
	{
		return fullName + "," + club;
	}

private:
};