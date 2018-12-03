using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMDuplicatesFinder
{
    class Staff
    {
        public string firstName = "";
        public string lastName = "";
        public string commonName = "";
        public string fullName = "";
        public string club = "";
        public DOB dob = new DOB("");

        public Staff(string firstName, string lastName, string commonName, string dobString, string club)
        {
            InitStaff(firstName, lastName, commonName, dobString, club);
        }

        public Staff(string[] exportedDbLine, int firstNameIndex, int lastNameIndex, int commonNameIndex, int dobIndex, int clubIndex)
        {
            InitStaff(exportedDbLine[firstNameIndex], exportedDbLine[lastNameIndex], exportedDbLine[commonNameIndex], exportedDbLine[dobIndex], exportedDbLine[clubIndex]);
        }

        public void InitStaff(string firstName, string lastName, string commonName, string dobString, string club)
        {
            this.firstName = firstName;
            this.lastName = lastName;
            this.commonName = commonName;
            fullName = firstName + " " + lastName;
            this.club = club;
            dob = new DOB(dobString);
        }

        public static bool Compare(Staff s1, Staff s2)
        {
            if(s1.fullName.Equals(s2.fullName) && DOB.Compare(s1.dob, s2.dob) && s1.club.Equals(s2.club))
            {
                return true;
            }
            return false;
        }

        public string print()
        {
            return fullName + "," + club;
        }
    }
}
