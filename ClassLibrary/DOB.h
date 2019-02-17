#pragma once

#include "DuplicateGroup.h"

public class DOB
{

public:	
	int day = 0;
	int month = 0;
	int year = 0;
	bool isValid = false;

	DOB() {
	}

	DOB(std::string dobString)
	{
		printable = dobString;
		if (dobString.length() == DOB_STRING_LENGTH)
		{			
			day = std::stoi(dobString.substr(DAY_INDEX, DAY_LENGTH), nullptr, 10);
			month = std::stoi(dobString.substr(MONTH_INDEX, MONTH_LENGTH), nullptr, 10);
			year = std::stoi(dobString.substr(YEAR_INDEX, YEAR_LENGTH), nullptr, 10);
		}		
		isValid = day > 0 && month > 0 && year > 0;
	}
	
	static int Compare(DOB d1, DOB d2)
	{
		if (!d1.isValid || !d2.isValid) {
			return DuplicateGroup::DIFFERENT;
		}
		if (d1.day == d2.day && d1.year == d2.year && d1.month == d2.month)
		{
			return DuplicateGroup::EQUAL;
		}
		if (d1.day != d2.day && d1.year != d2.year && d1.month != d2.month) {
			return DuplicateGroup::DIFFERENT;
		}
		if (d1.day == d2.day && d1.year == d2.year)
		{
			return DuplicateGroup::SIMILAR;
		}
		if (d1.day == d2.day && d1.month == d2.month)
		{
			return DuplicateGroup::SIMILAR;
		}
		if (d1.year == d2.year && d1.month == d2.month)
		{
			return DuplicateGroup::SIMILAR;
		}
		if (d1.year == d2.year) {
			//this handles cases like 31/10/1995 vs 01/11/1995 and considers them similar dates
			if (d1.day >= 27 && d2.day <= 2 && d2.month - d1.month == 1) {
				return DuplicateGroup::SIMILAR;
			}
			if (d2.day >= 27 && d1.day <= 2 && d1.month - d2.month == 1) {
				return DuplicateGroup::SIMILAR;
			}
		}
		return DuplicateGroup::DIFFERENT;
		//TODO maybe same year should be similar date becoz sometimes only year is input
	}

	std::string print()
	{
		return printable;
	}

private:
	static constexpr int DOB_STRING_LENGTH = 10;	
	static constexpr int DAY_INDEX = 0;
	static constexpr int MONTH_INDEX = 3;
	static constexpr int YEAR_INDEX = 6;
	static constexpr int DAY_LENGTH = 2;
	static constexpr int MONTH_LENGTH = 2;
	static constexpr int YEAR_LENGTH = 4;

	std::string printable;
};
