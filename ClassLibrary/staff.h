#pragma once

#include "DOB.h"

//using namespace System;

public class Staff
{

public:
	int id = -1;		
	std::string firstName = "";
	std::string lastName = "";
	std::string commonName = "";
	std::string firstNamePrintable = "";
	std::string lastNamePrintable = "";
	std::string commonNamePrintable = "";
	std::string fullName = ""; 
	std::string club = "";
	std::string loan = "";
	std::string clubPrintable = "";
	std::string loanPrintable = "";
	DOB dob;
	int duplicateGroupId[DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES]; //the duplicate group id he belongs to for each duplicate type, or -1 if none.	
	
	//Stores hashes of the strings for optimizing the equal comparisons (comparing two hashes is way faster than comparing each char between two strings)
	//TODO: to prevent false positives on hash comparisons, do some safe check in the strings lengths and in the first char of the strings?
	size_t firstNameH;
	size_t lastNameH;
	size_t commonNameH;
	size_t fullNameH;
	size_t clubH;
	size_t loanH;

	/*combines all field hashes into a single hash and returns this single hash value from this staff (for use in the pause/resume feature)*/
	size_t getHash() {
		std::hash<std::string> hasherS;		
		std::hash<int> hasherI;
		size_t ret = combineHash(hasherI(id), hasherS(dob.print()));
		ret = combineHash(ret, firstNameH);
		ret = combineHash(ret, lastNameH);
		ret = combineHash(ret, commonNameH);
		ret = combineHash(ret, fullNameH);
		ret = combineHash(ret, clubH);
		ret = combineHash(ret, loanH);
		//Console::WriteLine("staff hash: {0}", ret);
		return ret;
	}

	/*combines two hashes into a single hash using a multiply and an addition to scramble it a little*/
	static size_t combineHash(size_t h1, size_t h2) {
		return h1 * HASHER_MAGIC_NUMBER + h2;
	}

	Staff() {
		initDuplicateGroupIdArray();	
	}

	Staff(int index, std::string firstName, std::string lastName, std::string commonName, std::string dobString, std::string club, std::string loan)
	{
		id = index;					
				
		//stores original names too besides the normalized so we can print later
		for (size_t i = 0; i < firstName.length(); i++) {
			char c = getNormalizedChar(firstName[i]);
			if (c != 0) {
				this->firstName += c;
			}				
		}
		firstNamePrintable = firstName;

		for (size_t i = 0; i < lastName.length(); i++) {
			char c = getNormalizedChar(lastName[i]);
			if (c != 0) {
				this->lastName += c;
			}
		}
		lastNamePrintable = lastName;

		for (size_t i = 0; i < commonName.length(); i++) {
			char c = getNormalizedChar(commonName[i]);
			if (c != 0) {
				this->commonName += c;
			}
		}
		commonNamePrintable = commonName;				
		
		fullName = this->firstName + this->lastName;

		if (club.compare(NONE) == 0) {
			this->club = "";
			clubPrintable = "";
		} else {
			for (size_t i = 0; i < club.length(); i++) {
				char c = getNormalizedChar(club[i]);
				if (c != 0) {
					this->club += c;
				}
			}
			clubPrintable = club;
		}

		if (loan.compare(NONE) == 0) {
			this->loan = "";
			loanPrintable = "";
		}
		else {
			for (size_t i = 0; i < loan.length(); i++) {
				char c = getNormalizedChar(loan[i]);
				if (c != 0) {
					this->loan += c;
				}
			}
			loanPrintable = loan;
		}

		dob = DOB(dobString);

		initDuplicateGroupIdArray();
				
		std::hash<std::string> hasher;
		firstNameH = hasher(this->firstName);
		lastNameH = hasher(this->lastName);
		commonNameH = hasher(this->commonName);
		fullNameH = hasher(this->fullName);
		clubH = hasher(this->club);
		loanH = hasher(this->loan);
	}		

	/*compares two staffs and returns the duplicate type between them, or no type if not duplicates*/
	static int Compare(Staff s1, Staff s2)
	{		
		int dobComp = DOB::Compare(s1.dob, s2.dob);
		if (dobComp == DuplicateGroup::DIFFERENT) {
			int clubComp = CompareClub(s1.club, s2.club, s1.loan, s2.loan, s1.clubH, s2.clubH, s1.loanH, s2.loanH);
			if (clubComp == DuplicateGroup::DIFFERENT) {
				if (DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES > DuplicateGroup::DUPLICATE_TYPE_EDD) {
					int nameComp = CompareName(s1, s2, true);
					if (nameComp == DuplicateGroup::EQUAL) {
						return DuplicateGroup::DUPLICATE_TYPE_EDD;
					}
				}
				return DuplicateGroup::DUPLICATE_TYPE_NONE;
			}
			else {
				int nameComp = CompareName(s1, s2, true);
				if (nameComp == DuplicateGroup::EQUAL) {
					return DuplicateGroup::DUPLICATE_TYPE_EDE;
				}
				else {
					return DuplicateGroup::DUPLICATE_TYPE_NONE;
				}
			}
		}
		else {
			int clubComp = CompareClub(s1.club, s2.club, s1.loan, s2.loan, s1.clubH, s2.clubH, s1.loanH, s2.loanH);
			if (clubComp == DuplicateGroup::DIFFERENT) {
				int nameComp = CompareName(s1, s2, DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES <= DuplicateGroup::DUPLICATE_TYPE_SED);
				if (nameComp == DuplicateGroup::EQUAL) {
					if (dobComp == DuplicateGroup::SIMILAR) {
						return DuplicateGroup::DUPLICATE_TYPE_ESD;
					}
					else {
						return DuplicateGroup::DUPLICATE_TYPE_EED;
					}
				}
				else if(nameComp == DuplicateGroup::SIMILAR) {
					if (dobComp == DuplicateGroup::EQUAL) {
						return DuplicateGroup::DUPLICATE_TYPE_SED;
					}
				}
				else {
					return DuplicateGroup::DUPLICATE_TYPE_NONE;
				}
			}
			else {
				int nameComp = CompareName(s1, s2, false);
				if (nameComp == DuplicateGroup::DIFFERENT) {
					if (dobComp == DuplicateGroup::EQUAL) {
						return DuplicateGroup::DUPLICATE_TYPE_DEE;
					}
					else {
						return DuplicateGroup::DUPLICATE_TYPE_NONE;
					}
				}
				if (dobComp == DuplicateGroup::SIMILAR) {
					if (nameComp == DuplicateGroup::EQUAL) {
						return DuplicateGroup::DUPLICATE_TYPE_EDE;
					}
					else {
						return DuplicateGroup::DUPLICATE_TYPE_SSE;
					}
				}
				else {
					if (nameComp == DuplicateGroup::EQUAL) {
						return DuplicateGroup::DUPLICATE_TYPE_EEE;
					}
					else {
						return DuplicateGroup::DUPLICATE_TYPE_SEE;
					}
				}
			}
		}
		return DuplicateGroup::DUPLICATE_TYPE_NONE;		
		//julio baptista very interesting duplicate case in october 2018 database
		//and marquinhos (sao paulo and leverkussen) in lussenhof db another interesting case
	}

	std::string print()
	{
		return  + "FN: " + (firstNamePrintable.empty() ? NONE : firstNamePrintable) + "; LN: " + (lastNamePrintable.empty() ? NONE : lastNamePrintable) + "; CN: " + (commonNamePrintable.empty() ? NONE : commonNamePrintable) + "; DOB: " + dob.print() + "; CLUB: " + (clubPrintable.empty() ? NONE : clubPrintable) + "; LOAN: " + (loanPrintable.empty() ? NONE : loanPrintable) + "; STAFF ID: " + std::to_string(id);
	}

private:

	static constexpr char * NONE = "[None]";	

	static constexpr size_t HASHER_MAGIC_NUMBER = 3;//a number known to give good scrambling when combining hash numbers
	
	static constexpr float SIMILAR_PERCENTAGE = .75f;// .75 means the names need to be at least 75% similar to be considered similar
	static constexpr int MIDDLENAME_FACTOR = 4; //number of chars to look for in the extremities of a fullname, to identify cases where middle name is missing
	static constexpr int MIDDLENAME_MIN_LENGTH = MIDDLENAME_FACTOR * 2; //minnimun lenght of fullnames to check for in the missing middlename algorithm
	
	void initDuplicateGroupIdArray() {
		for (int i = 0; i < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES; i++) {
			duplicateGroupId[i] = -1;
		}
	}

	static char getNormalizedChar(char c) {
		if (c >= 97 && c <= 122) {//all lowercase ASCII letter chars, returns them unmodified
			return c;
		}
		if (c >= 65 && c <= 90) {//all uppercase ASCII letter chars, returns them lower case			
			return c + 32;
		}
		if (c == 36) {//'$' simbol returns 's'
			return 's';
		}
		if (c == 38) {//'&' simbol returns 'e'
			return 'e';
		}
		if (c == 64) {//'@' simbol returns 'a'
			return 'a';
		}
		if (c < 0) {
			/*all latin1 non-ASCII chars, discards them if they are whitespace or punctuation, 
			if they are similar to some ascii letter replaces them with the ascii letter,
			and if looks some unknown symbol replace it with '?' char for simplicity and for 
			automatically handling cases like Joþn Smith and JoÞn Smith*/			
			if (c < -95) { //all below -95 can be considered whitespace/punctuation and are discarded, except below:
				if (c == -128) {
					return 'e';
				}
				if (c == -125) {
					return 'f';
				}
				if (c == -122) {
					return 't';
				}
				if (c == -118 || c == -102) {
					return 's';
				}
				if (c == -114 || c == -98) {
					return 'z';
				}
				if (c == -97) {
					return 'y';
				}
			}
			else { // all above or equal -95
				if (c == -95 || (c >= -52 && c <= -49) || (c >= -20 && c <= -17)) {
					return 'i';
				}
				if (c == -94 || c == -57 || c == -25) {
					return 'c';
				}
				if (c == -93 || c == -92 || c == -90 || c == -87 || c == -82 || c == -80 || c == -79 || c == -74 || c == -70
					|| c == -34 || c == -2) {
					return '?';
				}
				if (c == -91 || c == -35 || c == -3 || c == -1) {
					return 'y';
				}
				if (c == -89) {
					return 's';
				}
				if (c == -86 || (c >= -64 && c <= -58) || (c >= -32 && c <= -26)) {
					return 'a';
				}
				if (c == -75 || (c >= -39 && c <= -36) || (c >= -7 && c <= -4)) {
					return 'u';
				}
				if (c >= -56 && c <= -53 || (c >= -24 && c <= -21)) {
					return 'e';
				}
				if (c == -48 || c == -16) {
					return 'd';
				}
				if (c == -47 || c == -15) {
					return 'n';
				}
				if ((c >= -46 && c <= -42) || c == -40 || (c >= -14 && c <= -10) || c == -8) {
					return 'o';
				}
				if (c == -41) {
					return 'x';
				}
				if (c == -33) {
					return 'b';
				}
			}
		}
		//All remaining non-letter chars are discarded for simplifiying and optimizing the algorythm
		return 0;
	}
		
	/**
	compares fullname, common name, first name and last name of two staffs to determine if the names can
	be considered equal, similar or different
	*/
	static int CompareName(Staff s1, Staff s2, bool abortIfNotEqual = false) {
		//if commonname of one is equal to full name of other, considers them as having equal names
		if (!s1.commonName.empty() && s1.commonNameH == s2.fullNameH) {
			return DuplicateGroup::EQUAL;
		}
		if (!s2.commonName.empty() && s2.commonNameH == s1.fullNameH) {
			return DuplicateGroup::EQUAL;
		}

		//if first and last name is inverted between both staff, considers their names as equal because its a very common editting mistake
		if (!s1.firstName.empty() && !s1.lastName.empty()) {
			if (s1.firstNameH == s2.lastNameH && s1.lastNameH == s2.firstNameH) {
				return DuplicateGroup::EQUAL;
			}
		}

		int s1len = s1.fullName.length();
		int s2len = s2.fullName.length();
		//compares full name of both staff
		if (s1len > 0 && s2len > 0) {
			if (s1len == s2len) {
				if (s1.fullNameH == s2.fullNameH) {
					//the full names are equal
					return DuplicateGroup::EQUAL;
				}

				if (abortIfNotEqual) {
					//abortIfNotEqual is for optimization purposes, its true when we only care if the names are equal or
					//different (i.e. when similar doesnt matter)
					return DuplicateGroup::DIFFERENT;
				}

				//both fullnames have the same lenght, so we compare them char by char to
				//see if the fullnames are equal, similar or different
				int subs = 0;
				int maxSubs = (int)((1.0f - SIMILAR_PERCENTAGE) * s1len);
				//TODO: this can have one very small optimization: we can break if theres less chars remaining than possible maxSubs being reached
				for (int i = 0; i < s1len; i++) {
					if (s1.fullName[i] != s2.fullName[i]) {
						subs++;
					}
					if (subs > maxSubs) {
						//too many differences, so we break
						break;
					}
				}
				if (subs <= maxSubs) {
					if (subs == 0) {
						//the full names are equal
						return DuplicateGroup::EQUAL;
					}
					//the full names are similar
					return DuplicateGroup::SIMILAR;
				}
				//the full names are different, so we now compare the common, first and last names to 
				//check	if the names can still be considered similar
			}
			else {
				//both fullnames have different lenghts, so they arent equal, but we
				//check if one of them may have a missing middle name and can be considered similar, so
				//this detects cases like "Manoel da Silva" vs "Manoel Pereira da Silva"
				if (s1len >= MIDDLENAME_MIN_LENGTH && s2len >= MIDDLENAME_MIN_LENGTH) {
					bool subFound = false;
					//checks if the first MIDDLENAME_FACTOR chars of both fullnames are equal
					for (int i = 0; i < MIDDLENAME_FACTOR; i++) {
						if (s1.fullName[i] != s2.fullName[i]) {
							//not equal, so we break
							subFound = true;
							break;
						}
					}
					if (!subFound) {
						//the first MIDDLENAME_FACTOR chars of both fullnames are equal, so now we check the last MIDDLENAME_FACTOR chars
						for (int i = 0; i < MIDDLENAME_FACTOR; i++) {
							if (s1.fullName[s1len - 1 - i] != s2.fullName[s2len - 1 - i]) {
								subFound = true;
								break;
							}
						}
					}
					if (!subFound) {
						//both fullnames can be considered similar as one seem to be missing the middle name but the rest seem to be equal
						return DuplicateGroup::SIMILAR;
					}
					//the full names are different, so we now compare the common, first and last names to 
					//check	if the names can still be considered similar
				}

				//TODO: perhaps implement some more advanced similar name algorithm, but it could cause very bad performance
				//TODO: maybe in middle name check we should simply pick the first and last words of the full name and compare them instead of first MIDDLENAME_FACTOR and last MIDDLENAME_FACTOR chars
			}
		}

		//compares common name of both staff, and if equal considers them as having similar name
		if (!s1.commonName.empty() && s1.commonNameH == s2.commonNameH) {
			return DuplicateGroup::SIMILAR;
		}
		//compares first name of both staff, and if equal considers them as having similar name
		if (!s1.firstName.empty() && s1.firstNameH == s2.firstNameH) {
			return DuplicateGroup::SIMILAR;
		}
		//compares last name of both staff, and if equal considers them as having similar name
		if (!s1.lastName.empty() && s1.lastNameH == s2.lastNameH) {
			return DuplicateGroup::SIMILAR;
		}

		//compares fullName of one with first and last name of other, and if equal considers them as having similar name
		if (!s1.fullName.empty() && (s1.fullNameH == s2.firstNameH || s1.fullNameH == s2.lastNameH)) {
			return DuplicateGroup::SIMILAR;
		}
		if (!s2.fullName.empty() && (s2.fullNameH == s1.firstNameH || s2.fullNameH == s1.lastNameH)) {
			return DuplicateGroup::SIMILAR;
		}
		//compares common name of one with first and last name of other, and if equal considers them as having similar name
		if (!s1.commonName.empty() && (s1.commonNameH == s2.firstNameH || s1.commonNameH == s2.lastNameH)) {
			return DuplicateGroup::SIMILAR;
		}
		if (!s2.commonName.empty() && (s2.commonNameH == s1.firstNameH || s2.commonNameH == s1.lastNameH)) {
			return DuplicateGroup::SIMILAR;
		}
		//compares first name of one with last name of other, and if equal considers them as having similar name
		if (!s1.firstName.empty() && s1.firstNameH == s2.lastNameH) {
			return DuplicateGroup::SIMILAR;
		}
		if (!s2.firstName.empty() && s2.firstNameH == s1.lastNameH) {
			return DuplicateGroup::SIMILAR;
		}

		//the names are different
		return DuplicateGroup::DIFFERENT;
	}

	static int CompareClub(std::string club1, std::string club2, std::string loan1, std::string loan2, size_t hc1, size_t hc2, size_t hl1, size_t hl2) {
		//TODO handle case of no club, maybe consider similar club?
		//TODO detect this type of club similarity: Nomme Kalju FC vs Nomme Kalju FC U21
		if (!club1.empty() && hc1 == hc2) {
			return DuplicateGroup::EQUAL;
		}
		if (!loan1.empty() && (hl1 == hl2 || hl1 == hc2)) {
			return DuplicateGroup::EQUAL;
		}
		if (!loan2.empty() && hl2 == hc1) {
			return DuplicateGroup::EQUAL;
		}
		return DuplicateGroup::DIFFERENT;
	}	

	//requires using namespace system:
	/*static void printAllPossibleChars () {
		for (int i = -128; i <= 127; i++) {
			char c = i;
			std::string x;
			x += c;
			
			Console::WriteLine("strange char table: {0}", c);
			String ^systemstring = gcnew String(x.c_str());
			Console::WriteLine("SYMBOL {0}", systemstring);
			delete systemstring;
		}
	}*/
};