#pragma once

#include <unordered_set>

/* A duplicate group contains the ids of staffs who are possible duplicates to each other in the given duplicate type */
public class DuplicateGroup
{	

public:
	/*there are 9 different duplicate types, they can be identified by the letters (E is Equal, S is similar and D is different
	and the first letter is for full name, second letter for date of birth and third letter for club). So, for example
	the ESD vector contains all duplicate groups of the staffs who have Equal name, Similar date of birth and Different club.
	But we use only 6 for now because the last 3 return too many false positives.*/	
	static constexpr int NUMBER_OF_DUPLICATE_TYPES = 6;

	//dont change the value of any duplicate type here because it will break the Staff::Compare function
	static constexpr int DUPLICATE_TYPE_MERGED = -2;
	static constexpr int DUPLICATE_TYPE_NONE = -1;
	static constexpr int DUPLICATE_TYPE_EEE = 0;
	static constexpr int DUPLICATE_TYPE_EED = 1;
	static constexpr int DUPLICATE_TYPE_EDE = 2;
	static constexpr int DUPLICATE_TYPE_SEE = 3;
	static constexpr int DUPLICATE_TYPE_SSE = 4;
	static constexpr int DUPLICATE_TYPE_ESD = 5;
	//TODO: maybe divide the EDE into ESE and EDE?
	//TODO: maybe include a SSD and a SDE type?
	//TODO: add option to include below types:
	//these 3 result in too many false positives, so they are disabled for now:
	static constexpr int DUPLICATE_TYPE_DEE = 6;
	static constexpr int DUPLICATE_TYPE_SED = 7;
	static constexpr int DUPLICATE_TYPE_EDD = 8;	

	static constexpr int DIFFERENT = -1;
	static constexpr int SIMILAR = 0;
	static constexpr int EQUAL = 1;

	int groupId;
	int duplicateType;	
	std::unordered_set<int> staffIds;

	DuplicateGroup(int groupId, int duplicateType) {
		this->groupId = groupId;
		this->duplicateType = duplicateType;		
	}

	void addStaffId(int id) {		
		staffIds.insert(id);
	}	

	static std::string getTypeTitle(int type) {		
		switch (type)
		{
		case DUPLICATE_TYPE_EEE:
			return "EEE: STAFFS WITH EQUAL NAMES, EQUAL DATES OF BIRTH AND EQUAL CLUBS";
		case DUPLICATE_TYPE_EED:
			return "EED: STAFFS WITH EQUAL NAMES, EQUAL DATES OF BIRTH AND DIFFERENT CLUBS";
		case DUPLICATE_TYPE_EDE:
			return "EDE: STAFFS WITH EQUAL NAMES, DIFFERENT DATES OF BIRTH AND EQUAL CLUBS";
		case DUPLICATE_TYPE_SEE:
			return "SEE: STAFFS WITH SIMILAR NAMES, EQUAL DATES OF BIRTH AND EQUAL CLUBS";
		case DUPLICATE_TYPE_SSE:
			return "SSE: STAFFS WITH SIMILAR NAMES, SIMILAR DATES OF BIRTH AND EQUAL CLUBS";
		case DUPLICATE_TYPE_ESD:
			return "ESD: STAFFS WITH EQUAL NAMES, SIMILAR DATES OF BIRTH AND DIFFERENT CLUBS";
		case DUPLICATE_TYPE_DEE:
			return "DEE: STAFFS WITH DIFFERENT NAMES, EQUAL DATES OF BIRTH AND EQUAL CLUBS";
		case DUPLICATE_TYPE_SED:
			return "SED: STAFFS WITH SIMILAR NAMES, EQUAL DATES OF BIRTH AND DIFFERENT CLUBS";
		case DUPLICATE_TYPE_EDD:
			return "EED: STAFFS WITH EQUAL NAMES, DIFFERENT DATES OF BIRTH AND DIFFERENT CLUBS";
		}		
		return "";
	}

	static std::string getGroupIdTitle(int type) {
		switch (type)
		{
		case DUPLICATE_TYPE_EEE:
			return "EEE-";
		case DUPLICATE_TYPE_EED:
			return "EED-";
		case DUPLICATE_TYPE_EDE:
			return "EDE-";
		case DUPLICATE_TYPE_SEE:
			return "SEE-";
		case DUPLICATE_TYPE_SSE:
			return "SSE-";
		case DUPLICATE_TYPE_ESD:
			return "ESD-";
		case DUPLICATE_TYPE_DEE:
			return "DEE-";
		case DUPLICATE_TYPE_SED:
			return "SED-";
		case DUPLICATE_TYPE_EDD:
			return "EDD-";
		}
		return "";
	}

private:
	//TODO: review when 3 in same group if 2 of them belong to other group what to do? what it is doing? It seems it correctly adds them to 2 groups as in:
	/*DUPLICATE GROUP ID SSE - 229:
	FN: J�zsef; LN: Nagy; CN: [None]; DOB: 01.01.1971; CLUB: Ny�regyh�za Spartacus; LOAN: [None]; STAFF ID : 58032
	FN : Tamas; LN: Nagy; CN: [None]; DOB: 18.01.1971; CLUB: Ny�regyh�za Spartacus; LOAN: [None]; STAFF ID : 61967
	FN : Tam�s; LN: Nagy; CN: [None]; DOB: 18.01.1971; CLUB: Ny�regyh�za Spartacus; LOAN: [None]; STAFF ID : 121643
	so if it is always like that then i think its great*/

	//TODO compare the non accents from transfertool with the accents to see which is better for my tool
};
