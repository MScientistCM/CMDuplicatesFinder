using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMDuplicatesFinder
{
    class DOB
    {
        private static int DOB_STRING_LENGTH = 10;
        private static int DOB_ARRAY_LENGTH = 3;
        private static int DAY_INDEX = 0;
        private static int MONTH_INDEX = 1;
        private static int YEAR_INDEX = 2;

        public int day = 0;
        public int month = 0;
        public int year = 0;

        public DOB(string dobString)
        {
            if (dobString.Length == DOB_STRING_LENGTH)
            {
                string[] splittedDOB = dobString.Split('.');
                if (splittedDOB.Length == DOB_ARRAY_LENGTH) 
                {
                    day = Convert.ToInt32(splittedDOB[DAY_INDEX]);
                    month = Convert.ToInt32(splittedDOB[MONTH_INDEX]);
                    year = Convert.ToInt32(splittedDOB[YEAR_INDEX]);
                }
            }
        }

        public bool IsValid()
        {
            return day > 0 && month > 0 && year > 0;
        }

        public static bool Compare(DOB d1, DOB d2)
        {
            if(d1.IsValid() && d1.day == d2.day && d1.month == d2.month && d1.year == d2.year)
            {
                return true;
            }
            return false;
        }
    }
}
