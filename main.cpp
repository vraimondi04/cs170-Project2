#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream>
#include <algorithm> 
#include <cmath> 

using namespace std;

void forwardSelection(vector<vector<long double>> trainingSet, int numFeatures) { 
  //vector of features with highest accuracy; our final answer
  vector<int> bestFeatures;
  //vector of features with highest accuracy in the current traversal
  vector<int> currBestFeatures;  
  float accuracyVal = 0.0;
  float highestAccuracy = 0.0;  
    
  for (int i = 1; i <= numFeatures - 1; ++i) {
    vector<int> tempHighAccuracyVals;
    float currentHighestA = 0.0; //current highest accuracy for this traversal

    //referenced Project 2_full_briefing.pptx slide 26-27
    for (int j = 1; j <= numFeatures; ++j) {
      vector<vector<long double>> tempSet; 
      vector<int> tempFeatures; 
      tempSet.push_back(trainingSet.at(0)); 
      tempFeatures = currBestFeatures; 
            
      for (int k = 0; k < tempFeatures.size(); ++k) { 
        tempSet.push_back( trainingSet.at(tempFeatures.at(k)) ); 
      } 
                
      //if the value of j is not found in the vector; assures that you won't compare features with themselves
      if (find(tempFeatures.begin(), tempFeatures.end(), j) == tempFeatures.end()) {
        //match the temporary set with the training set so they have corresponding rows to iterate through
        tempSet.push_back(trainingSet.at(j)); 
        tempFeatures.push_back(j); 
        cout << "Using { ";

        for (int i = 0; i < tempFeatures.size(); ++i) {
          cout << tempFeatures.at(i) << " "; 
        } 

        //function call for cross-validation function to calculate the accuracy
        accuracyVal = crossValidation(tempSet); 
        cout << "} Accuracy: " << accuracyVal << "%" << endl;

        if (accuracyVal > currentHighestA) { 
          currentHighestA = accuracyVal; 
          tempHighAccuracyVals = tempFeatures; 
        }
      }   
    } 
    currBestFeatures = tempHighAccuracyVals; 
    cout << "The best feature subset is: "; 

    for (int i = 0; i < currBestFeatures.size(); ++i) { 
      cout << currBestFeatures.at(i) << " "; 
    } 
      cout << "with an accuracy of " << currentHighestA << "%" << endl << endl; 

      //referenced Project 2_full_briefing.pptx slide 26-27
      if (currentHighestA > highestAccuracy) { 
        bestFeatures = currBestFeatures; 
        highestAccuracy = currentHighestA; 
        currBestFeatures = bestFeatures; 
      }
      else if (currentHighestA < highestAccuracy) { 
        cout << "Oh Neptune! Accuracy value is decreasing. Checking in case of local maxima." << endl; 
      }
  }
  cout << endl << "Finished search!" << endl; 
  cout << "The final best feature subset is: "; 
  for (int i = 0; i < bestFeatures.size(); ++i) { 
    cout << bestFeatures.at(i) << " "; 
  } 
  cout << "with an accuracy of " << highestAccuracy << "%" << endl; 
} 

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
