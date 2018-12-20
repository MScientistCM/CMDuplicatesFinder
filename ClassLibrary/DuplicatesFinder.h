#pragma once

//#include <vector>
#include <iomanip>
#include "staff.h"
#include "DuplicateGroup.h"

using namespace System;//TODO remove this line and all log prints before publishing this tool

public class DuplicatesFinder
{

public:

	/*parses the CSV stringstream and returns the number of staff found*/
	int parse(std::stringstream& wholeCsv) {
		Console::WriteLine("parse");

		//parses the whole csv stringstream		
		int staffCount = -1;
		while (!wholeCsv.eof()) {
			if (staffCount >= 0) {
				//adds staff to vector
				staffVector.push_back(getStaffFromLine(wholeCsv, staffCount));

				if (staffCount % 1000 == 0) {
					Console::WriteLine("Progress: staff count: {0}", staffCount);
				}
			}
			else {
				//skips header
				getStaffFromLine(wholeCsv, staffCount);
			}
			staffCount++;
		}
		Console::WriteLine("Parsed {0} staff", staffCount);

		//Initializes the duplicates vector. We use one vector of duplicate groups for each duplicate type.
		duplicates.reserve(DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES);
		for (int i = 0; i < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES; ++i)
		{
			std::vector<DuplicateGroup> emptyVector;
			duplicates.push_back(emptyVector);
		}

		//initializes the main iterator
		i = staffVector.begin();

		return staffCount;
	}

	/*finds some duplicates and returns the current list of duplicates as an string*/
	std::string findDuplicates() {		
		Console::WriteLine("findDuplicates");	

		if (firstIteration) {
			//returs an empty printable list in the first iteration so that the user can have an initial visual feedback
			firstIteration = false;
			return printableDuplicatesList(duplicates, staffVector, i->id);
		}
		
		//finds the duplicates
		int iterMax = i->id + MAX_ITERATIONS_WITHOUT_REPORTING;
		Console::WriteLine("currentStaffIterator: {0}, iterMax {1}", i->id, iterMax);
		for (bool foundDuplicate = false; i < staffVector.end() && i->id < iterMax && !foundDuplicate; ++i) {
			Console::WriteLine("Progress: staff i: {0}", i->id);
			if (i->id == iterMax - 1) {
				Console::WriteLine("Iter max reached. Progress: staff i: {0}", i->id);
			/*	duplicatesList = printDuplicatesList(duplicates, staffVector, i->id);
				String ^systemstring3 = gcnew String(duplicatesList.c_str());
				Console::WriteLine("{0}", systemstring3);
				delete systemstring3;*/
			}
			for (std::vector<Staff>::iterator j = i+1; j < staffVector.end(); ++j) {
				int duplicateType = Staff::Compare(*i, *j);

				if (duplicateType >= 0 && duplicateType < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES) {
					//then staff i is possible duplicate with staff j
					
					//sets this bool to true so that we save the updated duplicates list into the output file and
					//return to c# to update the visible list to the user
					foundDuplicate = true;

					/*Console::WriteLine("possible duplicate: {0}", duplicateType);
					String ^systemstring = gcnew String(i->print().c_str());
					Console::WriteLine("{0}", systemstring);
					delete systemstring;
					String ^systemstring2 = gcnew String(j->print().c_str());
					Console::WriteLine("{0}", systemstring2);
					delete systemstring2;*/

					if (i->duplicateGroupId[duplicateType] < 0 && j->duplicateGroupId[duplicateType] < 0) {
						//Console::WriteLine("none of them are in duplicate groups for this duplicate type");
						//none of them are in duplicate groups for this duplicate type, so we add them in a new duplicate group for this type
						int groupId = duplicates.at(duplicateType).size();
						DuplicateGroup dg = DuplicateGroup(groupId, duplicateType);
						dg.addStaffId(i->id);
						dg.addStaffId(j->id);
						duplicates.at(duplicateType).push_back(dg);
						i->duplicateGroupId[duplicateType] = groupId;
						j->duplicateGroupId[duplicateType] = groupId;
					}
					else if (i->duplicateGroupId[duplicateType] >= 0 && j->duplicateGroupId[duplicateType] >= 0) {						
						if (i->duplicateGroupId[duplicateType] != j->duplicateGroupId[duplicateType]) {
							//Console::WriteLine("both are in different duplicate groups for this duplicate type, so we merge their groups");
							//both are in different duplicate groups for this duplicate type, so we merge their groups
							for (std::unordered_set<int>::iterator it = duplicates.at(duplicateType).at(j->duplicateGroupId[duplicateType]).staffIds.begin(); 
								it != duplicates.at(duplicateType).at(j->duplicateGroupId[duplicateType]).staffIds.end(); ++it)
							{
								duplicates.at(duplicateType).at(i->duplicateGroupId[duplicateType]).addStaffId(*it);
								staffVector.at(*it).duplicateGroupId[duplicateType] = i->duplicateGroupId[duplicateType];
							}
							duplicates.at(duplicateType).at(j->duplicateGroupId[duplicateType]).duplicateType = DuplicateGroup::DUPLICATE_TYPE_MERGED;
						}
						else {
							//Console::WriteLine("both are in the same duplicate group for this duplicate type already, so we do nothing");
							//both are in the same duplicate group for this duplicate type already, so we do nothing
						}
					}
					else if (i->duplicateGroupId[duplicateType] >= 0) {
						//Console::WriteLine("only the staff 'i' is in a duplicate group for this duplicate type, so we add staff 'j' to the same group");
						//only the staff 'i' is in a duplicate group for this duplicate type, so we add staff 'j' to the same group
						duplicates.at(duplicateType).at(i->duplicateGroupId[duplicateType]).addStaffId(j->id);
						j->duplicateGroupId[duplicateType] = i->duplicateGroupId[duplicateType];
					}
					else {
						//Console::WriteLine("only the staff 'j' is in a duplicate group for this duplicate type, so we add staff 'i' to the same group");
						//only the staff 'j' is in a duplicate group for this duplicate type, so we add staff 'i' to the same group
						duplicates.at(duplicateType).at(j->duplicateGroupId[duplicateType]).addStaffId(i->id);
						i->duplicateGroupId[duplicateType] = j->duplicateGroupId[duplicateType];
					}

					/*duplicatesList = printDuplicatesList(duplicates, staffVector, i->id);					
					String ^systemstring3 = gcnew String(duplicatesList.c_str());
					Console::WriteLine("{0}", systemstring3);
					delete systemstring3;*/
				}				 
			}
		}		
		
		return printableDuplicatesList(duplicates, staffVector, i->id - 1);
	}


private:
	//hardcoded values as per the csv generated by the TransferTool app
	static constexpr int firstNameIndex = 0;
	static constexpr int lastNameIndex = 1;
	static constexpr int commonNameIndex = 2;
	static constexpr int dobIndex = 3;
	static constexpr int clubIndex = 9;	
	static constexpr int loanIndex = 10;

	//When more than this number of iterations is reached without finding any new duplicates,
	//we return to c# layer to update the progress bar
	static constexpr int MAX_ITERATIONS_WITHOUT_REPORTING = 50;	
	
	//If change this constant here, then the UI in c# side needs to be manually changed as well to reflect the new filename
	static constexpr char * RESULT_FILENAME = "DuplicatesList.txt";

	std::vector<Staff> staffVector;
	std::vector<std::vector<DuplicateGroup>> duplicates;
	std::vector<Staff>::iterator i;
	bool firstIteration = true;

	static std::string printableDuplicatesList(std::vector<std::vector<DuplicateGroup>> duplicates, std::vector<Staff> staffVector, int currentStaffId) {
		//TODO: sort result by likelly chances with a percentage score based on their similarity substitutions
		bool isCompleted = false;
		float progress = 100.0;
		std::string internalUse;
		std::string result;

		/*the first two lines of the result string are for internal use by the code, it isn't print to the user, its just
		to pass progress information between c++ and c# layer*/
		if (staffVector.size() == currentStaffId + 1) {
			isCompleted = true;
			internalUse += "1";
		}
		else {
			internalUse += "0";
		}
		internalUse += "\n" + std::to_string(currentStaffId) + "\n";

		result += "DUPLICATES LIST - ";
		if (isCompleted) {
			result += "FULL ";			
		}
		else {
			result += "PARTIAL ";
			progress = ((float)currentStaffId / (float)staffVector.size()) * 100.0f;			
		}
		result += "RESULT (";
		if (!isCompleted) {
			std::stringstream floatStream;
			floatStream << std::fixed << std::setprecision(2) << progress;			
			result += "PROGRESS COMPLETED SO FAR: " + floatStream.str() + "% : ";
		}
		int duplicatesFound = 0;
		for (int i = 0; i < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES; ++i)
		{
			std::vector<DuplicateGroup> groupVector = duplicates.at(i);
			duplicatesFound += groupVector.size();
		}
		result += std::to_string(duplicatesFound) + " DUPLICATE GROUPS FOUND";
		if (!isCompleted) {
			result += " SO FAR";
		}
		result += "):\r\n";
		result += "Legend: (FN = First name; LN = Last name; CN = Common name; DOB = Date of birth)\r\n";	
		 
		for (int i = 0; i < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES; ++i)
		{
			result += "\r\n\r\n************************* DUPLICATE TYPE " + DuplicateGroup::getTypeTitle(i) + " (";
			std::vector<DuplicateGroup> groupVector = duplicates.at(i);
			result += std::to_string(groupVector.size()) + " DUPLICATE GROUPS FOUND";
			if (!isCompleted) {
				result += " SO FAR";
			}
			result += "):\r\n";
			for (std::vector<DuplicateGroup>::iterator j = groupVector.begin(); j != groupVector.end(); ++j) {
				if (j->duplicateType >= 0) {
					result += "    DUPLICATE GROUP ID " + DuplicateGroup::getGroupIdTitle(i) + std::to_string(j->groupId) + ":\r\n";
					for (std::unordered_set<int>::iterator k = j->staffIds.begin(); k != j->staffIds.end(); ++k) {
						result += "        " + staffVector.at(*k).print() + "\r\n"; 
					}
				}
			}			
		}

		std::ofstream resultFile(RESULT_FILENAME, std::ios::binary);
		resultFile << result;
		resultFile.close();

		return internalUse + result;
	}

	static Staff getStaffFromLine(std::stringstream& wholeCsv, int staffCount)
	{		
		std::string line;		
		std::getline(wholeCsv, line);

		if (staffCount < 0) {
			//ignores header
			Staff staff;
			return staff;
		}

		std::string fn, ln, cn, dob, club, loan;
		
		std::istringstream lineStream(line);				
		
		int index = 0;
		std::string cell;
		while (std::getline(lineStream, cell, ';') && index <= loanIndex)
		{
			if (index == firstNameIndex) {
				fn = cell;
			}
			else if (index == lastNameIndex) {
				ln = cell;
			}
			else if (index == commonNameIndex) {
				cn = cell;
			}
			else if (index == dobIndex) {
				dob = cell;
			}
			else if (index == clubIndex) {
				club = cell;
			}		
			else if (index == loanIndex) {
				loan = cell;
			}
			index++;
		}
		/*// checks for a trailing comma with no data after it
		if (!lineStream && cell.empty())
		{
			// if there was a trailing comma then adds an empty element
			staff.push_back("");
		}*/		
		return Staff(staffCount, fn, ln, cn, dob, club, loan);
	}
};
