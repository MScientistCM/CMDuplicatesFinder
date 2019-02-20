#pragma once

#include "DOB.h"
#include <algorithm>

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

	//TODO:maybe use two numbers: one .75 to consider equal, one .60 to consider similar
	static constexpr float SIMILAR_PERCENTAGE = .75f;// .75 means the names need to be at least 75% similar to be considered equal
	static constexpr int MIDDLENAME_FACTOR = 4; //number of chars to look for in the extremities of a fullname, to identify cases where middle name is missing
	static constexpr int MIDDLENAME_MIN_LENGTH = MIDDLENAME_FACTOR * 2; //minnimun lenght of fullnames to check for in the missing middlename algorithm
	
	void initDuplicateGroupIdArray() {
		for (int i = 0; i < DuplicateGroup::NUMBER_OF_DUPLICATE_TYPES; i++) {
			duplicateGroupId[i] = -1;
		}
	}

	//TODO: maybe make 0 equal to o, i equal to y and v equal to w, c q qu ck and k equal (but c could equal ss too), ks and x equal (but x could equal z too) and this type of similar char substitution
	//TODO: maybe make double chars like ss equal to single s, rr to r, etc, to detect tipos like alessandro vs alesandro
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
	returns true if the strings aren't empty and are similar by SIMILAR_PERCENTAGE or more in distance,
	returns false otherwise
	both strings should be lower case
	
	Its an implementation of the Damerau-Levenshtein algorithm based on https://github.com/PierreBoyeau/levenshtein_distance
		
	//TODO: after each levenshtein comparison, store the result in a table so that when 2 strings are levenshtein compared then first check their hashes in the table to see if they were already compared to skip levenshtein comparison
	*/
	static bool isLevenshteinEqual(std::string s1, std::string s2, bool isFullName = false)
	{
		int len1 = s1.length();
		int len2 = s2.length();
		int maxDist = getMaxEditDistance(len1, len2, isFullName);

		if (maxDist <= 0) {
			return false;
		}
		
		int suppr_dist, insert_dist, subs_dist, val;
		int* dist = new int[(len1 + 1)*(len2 + 1)];

		for (int i = 0; i < len1 + 1; ++i)
			dist[(len2 + 1)*i] = i;
		for (int j = 0; j < len2 + 1; ++j)
			dist[j] = j;
		for (int i = 1; i < len1 + 1; ++i) {
			for (int j = 1; j < len2 + 1; ++j) {
				suppr_dist = dist[(len2 + 1)*(i - 1) + j] + 1;
				insert_dist = dist[(len2 + 1)*i + j - 1] + 1;
				subs_dist = dist[(len2 + 1)*(i - 1) + j - 1];
				if (s1[i - 1] != s2[j - 1])  // word indexes are implemented differently.
					subs_dist += 1;
				val = std::min(suppr_dist, std::min(insert_dist, subs_dist));
				if (((i >= 2) && (j >= 2)) && ((s1[i - 1] == s2[j - 2]) && (s1[i - 2] == s2[j - 1])))
					val = std::min(dist[(len2 + 1)*(i - 2) + j - 2] + 1, val);
				dist[(len2 + 1)*i + j] = val;
			}
		}

		int res = dist[(len1 + 1)*(len2 + 1) - 1];
		delete dist;		
		return res <= maxDist;
	}

	/**
	given the lengths of 2 strings, returns the greater than 0 max allowed edit distance based on the SIMILAR_PERCENTAGE,
	or returns 0 if its sure the strings cant be considered equal given their lengths
	*/
	static int getMaxEditDistance(int len1, int len2, bool isFullName = false) {
		//returns false if length of one string is equal to 0		
		if (len1 <= 0 || len2 <= 0) {
			return 0;
		}		

		int maxLen = std::max(len1, len2);
		int minLen = std::min(len1, len2);

		//calculates the max distance they can have to be considered equal based on SIMILAR_PERCENTAGE		
		int maxDist = (int)((1.0f - SIMILAR_PERCENTAGE) * maxLen);
		
		//max distance lower than 1 means the strings are too small to be compared based on SIMILAR_PERCENTAGE, so we return 0
		if (maxDist < 1) {			
			return 0;
		}
		
		if (isFullName && maxDist > 1) {
			//when the names being compared are full names we divide the max allowed Levenshtein distance by two to prevent lots of false positives
			maxDist = maxDist / 2;

			//TODO: how to detect these cases as having equal names instead of similar, but preventing false positives like Dani vs Javi Castellano and Rui vs Luis Silva?:
				//FN: Baggio; LN: Rakotonomenjanahary; CN: [None]; DOB: 19.12.1974; CLUB: Sukhothai FC; LOAN: [None]; STAFF ID: 2909
				//FN: John Baggio; LN: Rakotonomenjanahary; CN: John Baggio; DOB: 19.12.1974; CLUB: Sukhothai FC; LOAN: [None]; STAFF ID : 135149
				//FN: Maxim; LN: Kirsanov; CN: [None]; DOB: 08.05.1970; CLUB: Vityaz Podolsk; LOAN: [None]; STAFF ID : 2345
				//FN : Maksim; LN: Kirsanov; CN: [None]; DOB: 08.05.1970; CLUB: FC Zugdidi; LOAN: [None]; STAFF ID : 139819
				//FN: Yassine; LN: Bezzaz; CN: [None]; DOB: 10.07.1964; CLUB: MC El Eulma; LOAN: [None]; STAFF ID : 1183
				//FN : Yacine; LN: Bezzaz; CN: [None]; DOB: 09.07.1964; CLUB: CS Constantine; LOAN: [None]; STAFF ID : 114431
				//I think the answer is: add an option to be more aggressive in the algorithm, so it will find more duplicates at the cost of finding more false positives
				//So, when the user enables that option, it will simply skip this 'if' check
		}

		//if the difference in lenghts between both strings are greater than maxDist, we are sure they wont be considered equal, so we return 0
		if (maxLen - minLen > maxDist) {
			return 0;
		}

		return maxDist;
	}

	/**
	returns true if the common name of s2, when Levenshtein compared with the full name of s1, can be considered equal, returns false otherwise
	*/
	static bool isCommonFullLevEqual(Staff s1, Staff s2) {	
		//instead of simply comparing the common name of one with the full name of other via Levenshtein, 
		//we split the common name in two and compare it twice, one time the left portion with the first name and another time
		//the right portion with the last name, otherwise it would produce too many false positives		
		int s2CNLen = s2.commonName.length();
		int s1FNLen = s1.firstName.length();
		int s1LNLen = s1.lastName.length();

		if (s1FNLen <= 0 || s1LNLen <= 0) {
			return false;
		}

		if (s2CNLen > s1FNLen && s2CNLen > s1LNLen) {
			if (getMaxEditDistance(s1.fullName.length(), s2CNLen) <= 0) {
				return false;
			}						
					
			if (isLevenshteinEqual(s2.commonName.substr(0, s1FNLen), s1.firstName)
				&&
				isLevenshteinEqual(s2.commonName.substr(s1FNLen, s2CNLen - s1FNLen), s1.lastName)) {
				return true;
			}
			//TODO: if one of the two levenshtein comparisons above is true but the other is false, we could
			//consider the names as being at least similar			
		}
		return false;
	}	
			
	/**
	compares fullname, common name, first name and last name of two staffs to determine if the names can
	be considered equal, similar or different
	*/
	static int CompareName(Staff s1, Staff s2, bool abortIfNotEqual = false) {
		//if common name of one is equal to full name of other, considers them as having equal names
		if (!s1.commonName.empty() && s1.commonNameH == s2.fullNameH) {
			return DuplicateGroup::EQUAL;
		}
		if (!s2.commonName.empty() && s2.commonNameH == s1.fullNameH) {
			return DuplicateGroup::EQUAL;
		}

		//compares full name of both staff
		if (!s1.fullName.empty() && s1.fullNameH == s2.fullNameH) {
			//the full names are equal
			return DuplicateGroup::EQUAL;
		}		
		
		//if first and last name is inverted between both staff, considers their names as equal because its a very common editting mistake
		if (!s1.firstName.empty() && !s1.lastName.empty()) {			
			if (s1.firstNameH == s2.lastNameH && s1.lastNameH == s2.firstNameH) {
				return DuplicateGroup::EQUAL;
			}			
			if (
				(s1.firstNameH == s2.lastNameH || isLevenshteinEqual(s1.firstName, s2.lastName))
				&&
				(s1.lastNameH == s2.firstNameH || isLevenshteinEqual(s1.lastName, s2.firstName))
				) {
				return DuplicateGroup::EQUAL;
			}			
		}		

		//now compare the common with full names as we did above, but now via levenshtein checks, to see if the names can still be considered equal			
		if (isCommonFullLevEqual(s1, s2) || isCommonFullLevEqual(s2, s1)) {
			return DuplicateGroup::EQUAL;
		}

		//compares full names via levenshtein passing true as a parameter to be more conservative and avoid too many false positives
		if (isLevenshteinEqual(s1.fullName, s2.fullName, true)) {
			return DuplicateGroup::EQUAL;
		}

		//compares full names by checking first name and last name separatelly in levenshtein, instead 
		//of single checking the concatenated first name + last name as we did above
		//because it avoids false positives when first or last name is too small (due to SIMILAR_PERCENTAGE being used instead of a hardcoded maxDistance)
		//(ex: Javi Castellano vs Dani Castellano would be a false positive that is prevented with this strategy)
		bool levEqualFN = !s1.firstName.empty() && (s1.firstNameH == s2.firstNameH || isLevenshteinEqual(s1.firstName, s2.firstName));
		bool levEqualLN = !s1.lastName.empty() && (s1.lastNameH == s2.lastNameH || isLevenshteinEqual(s1.lastName, s2.lastName));
		if (
			(levEqualFN && levEqualLN) 
			|| 
			(levEqualFN && s1.lastName.empty() && s2.lastName.empty()) 
			||
			(levEqualLN && s1.firstName.empty() && s2.firstName.empty())
			) {
			return DuplicateGroup::EQUAL;
		}

		if (abortIfNotEqual) {
			//abortIfNotEqual is for optimization purposes, its true when we only care if the names are equal or
			//different (i.e. when similar doesnt matter)
			return DuplicateGroup::DIFFERENT;
		}		

		//the names cant be considered equal, so now we check if they can be considered similar,
		//first comparing hashes only, then comparing via levenshtein

		//compares first name of both staff, and if equal considers them as having similar name
		if (levEqualFN) {
			return DuplicateGroup::SIMILAR;
		}
		//compares last name of both staff, and if equal considers them as having similar name
		if (levEqualLN) {
			return DuplicateGroup::SIMILAR;
		}
		//compares common name of both staff, and if equal considers them as having similar name
		if (!s1.commonName.empty() && s1.commonNameH == s2.commonNameH) {
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

		//check if one of them may have a missing middle name and can be considered similar, so
		//this detects cases like "Manoel da Silva" vs "Manoel Pereira da Silva"
		int s1len = s1.fullName.length();
		int s2len = s2.fullName.length();
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
			//TODO: maybe in this middle name check we should simply pick the first and last words of the full name and compare them instead of first MIDDLENAME_FACTOR and last MIDDLENAME_FACTOR chars
			//TODO: maybe this middle name check should return exactly how many equal chars in begin and end so that we can decide with a threashold if the number is big they will be considered equal. This is to pick cases like: 
				/*FN: Farid Abdel; LN: Zato - Arouna; CN: [None]; DOB: 24.04.1975; CLUB: [None]; LOAN: [None]; STAFF ID : 1470
				FN : Farid; LN: Zato - Arouna; CN: [None]; DOB: 24.04.1975; CLUB: UMF Víkingur Ólafsvík; LOAN: [None]; STAFF ID : 111797*/
		}

		
		//hashes compared, so now we repeat the same comparisons but using levenshtein		
		if (isLevenshteinEqual(s1.commonName, s2.commonName)) {
			return DuplicateGroup::SIMILAR;
		}			
		if (isLevenshteinEqual(s1.fullName, s2.firstName) || isLevenshteinEqual(s1.fullName, s2.lastName)) {
			return DuplicateGroup::SIMILAR;
		}
		if (isLevenshteinEqual(s2.fullName, s1.firstName) || isLevenshteinEqual(s2.fullName, s1.lastName)) {
			return DuplicateGroup::SIMILAR;
		}		
		if (isLevenshteinEqual(s1.commonName, s2.firstName) || isLevenshteinEqual(s1.commonName, s2.lastName)) {
			return DuplicateGroup::SIMILAR;
		}
		if (isLevenshteinEqual(s2.commonName, s1.firstName) || isLevenshteinEqual(s2.commonName, s1.lastName)) {
			return DuplicateGroup::SIMILAR;
		}		
		if (isLevenshteinEqual(s1.firstName, s2.lastName)) {
			return DuplicateGroup::SIMILAR;
		}
		if (isLevenshteinEqual(s2.firstName, s1.lastName)) {
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