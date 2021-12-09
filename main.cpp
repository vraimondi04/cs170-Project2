#include <iostream>

using namespace std;

int main() {
  
  //user input and initial output
  string fileName;
  int userAlgorithm; 
  cout << "Welcome to Vincent Raimondi's Feature Selection Algorithm." << endl << "Type in the name of the file to test: " << endl; 
  cin >> fileName; 
  cout << "Select the algorithm you want to run on this file:" << endl << "1) Forward Selection" << endl << "2) Backward Elimination" << endl; 
  cin >> userAlgorithm; 
	
  //file variables to read in the small/large datasets; reference cs012 for reading and writing to files in c++
	ifstream fin; 
  fin.open(fileName);

  //safety net if file cannot be opened; end program
  if (!fin.is_open()) {
    cout << "Error opening file." << endl; 
    return -1; 
  }
  
  return 0;
} 
