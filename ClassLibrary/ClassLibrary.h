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

		String^ GetDuplicates(String^ inputFilename) {
			Console::WriteLine("GetDuplicates inputFilename:");
			Console::WriteLine(inputFilename);
						
			std::stringstream wholeCsv;

			//converts the String^ to char*
			IntPtr ptrToNativeString = Marshal::StringToHGlobalAnsi(inputFilename);
			
			try {
				//opens the file
				std::ifstream myfile(static_cast<char*>(ptrToNativeString.ToPointer()));

				if (myfile.is_open()) {
					Console::WriteLine("File opened");

					//reads the whole csv at once into an stringstream
					wholeCsv << myfile.rdbuf();
					Console::WriteLine("File read");

					myfile.close();
					Console::WriteLine("File closed");
				}
				else {
					Console::WriteLine("Couldnt open the file");
				}
			}
			catch (...)
			{
				Console::WriteLine("Exception");
				Marshal::FreeHGlobal(ptrToNativeString);					
				throw;
			}
			Marshal::FreeHGlobal(ptrToNativeString);			
			
			std::string duplicates = DuplicatesFinder::findDuplicates(wholeCsv);
			
			//converts from std string to managed string
			String^ systemstring = gcnew String(duplicates.c_str());
			
			Console::WriteLine(systemstring);									
			
			return systemstring; 
		}
	

	private:
	};


}
