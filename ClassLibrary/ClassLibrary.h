#pragma once

#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


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
			
			//parses the whole csv stringstream
			int staffCount = 0;
			while (!wholeCsv.eof()) {
				std::vector<std::string> line = getNextLineAndSplitIntoTokens(wholeCsv);
				staffCount++;
				//TODO: process each line

				if (staffCount % 1000 == 0) {
					Console::WriteLine("Progress: staff count: {0}", staffCount);
					for (auto i : line) {
						String ^systemstring = gcnew String(i.c_str());
						Console::WriteLine("{0}", systemstring);
						delete systemstring;
					}
				}				
			}
			
			//return gcnew String("Hello World!");
			return inputFilename; 
		}
	

	private:		

		std::vector<std::string> getNextLineAndSplitIntoTokens(std::stringstream& input)
		{
			std::vector<std::string> result;
			std::string line;			
			std::string cell;

			std::getline(input, line);

			std::istringstream lineStream(line);			

			while (std::getline(lineStream, cell, ';'))
			{
				result.push_back(cell);
			}
			// checks for a trailing comma with no data after it
			if (!lineStream && cell.empty())
			{
				// if there was a trailing comma then adds an empty element
				result.push_back("");
			}
			return result;
		}
	};


}
