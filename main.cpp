#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream>

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
  
  //vectors to store the set of data from the file
  vector<vector<long double>> trainingSet; 
  vector<long double> featureList;
  string row; 
  string feature;
  int numFeatures;
    
  getline(fin, row); 
  stringstream sstream(row); 

  //while there are still features to grab from the current row
  while (sstream >> feature) { 
    featureList.push_back(stold(feature)); //string to long double; std::stold
    trainingSet.push_back(featureList); 
    featureList.clear(); 
  } 

  //while there are still rows in the file to get features from (while there are still instances left)
  while (getline(fin, row)) { 
    stringstream sstream(row); 
    
    for (int i = 0; i < trainingSet.size(); i++) { 
      if (sstream >> feature) {
        trainingSet.at(i).push_back(stold(feature)); //string to long double; std::stold
      }  
    } 
  } 
  
  fin.close(); 
	
  //rest of output after obtaining dataset from the file
  numFeatures = trainingSet.size() - 1; 
  cout << "This dataset has " << numFeatures << " features, alongside " << trainingSet.at(0).size() << " instances." << endl; 
  cout << "Running nearest neighbor with all " << numFeatures << " features, using \"leaving-one-out\" evaluation, I get an accuracy of "; 
  cout << crossValidation(trainingSet) << "%" << endl; 
	
  if (userAlgorithm == 1) {
    forwardSelection(trainingSet, numFeatures);
  }  
  else if (userAlgorithm == 2) {
    backwardSelection(trainingSet, numFeatures);
  } 
  
  return 0;
} 
