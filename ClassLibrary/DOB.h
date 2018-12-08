#pragma once

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
		if (dobString.length() == DOB_STRING_LENGTH)
		{			
			day = std::stoi(dobString.substr(0,2), nullptr, 10);
			month = std::stoi(dobString.substr(3, 2), nullptr, 10);
			year = std::stoi(dobString.substr(6, 4), nullptr, 10);
		}
		isValid = day > 0 && month > 0 && year > 0;
	}
	
	static bool Compare(DOB d1, DOB d2)
	{
		if (d1.isValid && d1.day == d2.day && d1.month == d2.month && d1.year == d2.year)
		{
			return true;
		}
		return false;
	}

private:
	static constexpr int DOB_STRING_LENGTH = 10;
	static constexpr int DOB_ARRAY_LENGTH = 3;
	static constexpr int DAY_INDEX = 0;
	static constexpr int MONTH_INDEX = 1;
	static constexpr int YEAR_INDEX = 2;
};
