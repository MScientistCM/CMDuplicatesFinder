namespace CMDuplicatesFinder
{
    class ProgressReporter
    {
        private int totalStaff = 0;
        private string currentResult = "";
        private string status = "";        

        public ProgressReporter(string status)
        {            
            this.status = status;
        }

        public void UpdateResult(string newResult)
        {
            currentResult = newResult;
        }

        public string GetResult()
        {
            return currentResult;
        }

        public void SetStatus(string status)
        {
            this.status = status;
        }

        public string GetStatus()
        {
            return status;
        }

        public void SetTotalStaff(int totalStaff)
        {
            this.totalStaff = totalStaff;
        }

        public int GetTotalStaff()
        {
            return totalStaff;
        }
    }
}
