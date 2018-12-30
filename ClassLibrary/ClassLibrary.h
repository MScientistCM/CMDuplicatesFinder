#pragma once

#include <fstream>
#include <sstream>
#include "DuplicatesFinder.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ClassLibrary {	
	public ref class Class1
	{
		
	public:	
		
		~Class1() {
			Console::WriteLine("Class1 destructor");
			releaseAllResources();
		}

		void releaseAllResources() {
			Console::WriteLine("Class1 releaseAllResources");
			releaseCSV();
			releaseDupFinder();
		}

		/*opens the csv file and reads it into memory at once, and
		returns true if it was successful or false otherwise*/
		bool ReadCSVFile(String^ inputFilename) {
			Console::WriteLine("ReadCSVFile inputFilename:");
			Console::WriteLine(inputFilename);		

			//converts the String^ to char*
			IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(inputFilename);
			
			bool ret = false;
			try {
				//opens the file
				std::ifstream myfile(static_cast<char*>(ptrToNativeString.ToPointer()));

				if (myfile) {
					Console::WriteLine("File opened");

					//reads the whole csv at once into an stringstream			
					wholeCsv = new std::stringstream();
					*wholeCsv << myfile.rdbuf();
					Console::WriteLine("File read");

					myfile.close();
					Console::WriteLine("File closed");
					ret = true;
				}
				else {
					Console::WriteLine("Couldnt open the file");
				}
			}
			catch (...)
			{
				Console::WriteLine("Exception");
				Marshal::FreeHGlobal(ptrToNativeString);	
				ret = false;
				throw;
			}
			Marshal::FreeHGlobal(ptrToNativeString);	

			Console::WriteLine("returning");
			return ret; 
		}

		/*parses the CSV stringstream and returns the number of staff found*/
		int ParseCSV() {
			Console::WriteLine("ParseCSV");			

			dupFinder = new DuplicatesFinder();
			int totalStaff = dupFinder->parse(*wholeCsv);
			releaseCSV();			

			return totalStaff;
		}
	
		/*finds some duplicates and returns the current list of duplicates as an string*/
		String^ GetDuplicates() {
			Console::WriteLine("GetDuplicates");			

			/*std::string duplicates = dupFinder->findDuplicates();

			//converts from std string to managed string
			String^ systemstring = gcnew String(duplicates.c_str());

			Console::WriteLine(systemstring);

			return systemstring;*/
			
			return gcnew String(dupFinder->findDuplicates().c_str());
		}


	private:

		std::stringstream* wholeCsv;
		DuplicatesFinder* dupFinder;

		void releaseCSV() {
			Console::WriteLine("Class1 releaseCSV");
			delete wholeCsv;
			wholeCsv = NULL;
		}

		void releaseDupFinder() {
			Console::WriteLine("Class1 releaseDupFinder");
			delete dupFinder;
			dupFinder = NULL;
		}
	};


}
