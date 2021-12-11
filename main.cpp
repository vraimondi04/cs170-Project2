#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream>
#include <algorithm> 
#include <cmath> 

using namespace std;

int nearestNeighbor(vector<long double> instance, vector<vector< long double>> subset) { 
  //first element for each row (the first column is the class of that instance; either 1 or 2)
  int classVal = instance.at(0); 
  //minimum distance value out of all of the options in the subset
  long double minVal = 0;

  //nested for loop traverses the training subset to calculate the minimum distance 
  for (int i = 0; i < subset.at(0).size(); ++i) { 
    long double distanceVal = 0.0; 

    for (int j = 1; j < subset.size(); j++) { 
      //reference the distance function to calculate distance value
      distanceVal += pow(subset.at(j).at(i) - instance.at(j), 2);
    } 

    //initial condition for the minimum distance value
    if (i == 0) {
      minVal = distanceVal;
    } 
    else if (distanceVal < minVal) {
      minVal = distanceVal; 
      classVal = subset.at(0).at(i); 
    }
  } 
  //returning the integer value corresponding to which class it belongs to; in our project it will either be 1 or 2
  return classVal; 
} 

//reference the accuracy function in the project 2 briefing for this function
float crossValidation(vector<vector<long double>> trainingSet) { 
  vector<vector<long double>> instanceSubset; 
  vector<long double> currInstance; 
  float numCorrect = 0;

  //traversing the different options for the current subset of the data
  for (int i = 0; i < trainingSet.at(0).size(); ++i) { 
    for (int j = 0; j < trainingSet.size(); ++j) {

      //copy all possible features to combine with the latest subset and compare it with the current instance
      vector<long double> tempFeature = trainingSet.at(j); 

      currInstance.push_back(tempFeature.at(i));
      tempFeature.erase(tempFeature.begin() + i);
      instanceSubset.push_back(tempFeature); 
    }
    //referenced Project 2_full_briefing.pptx slide 39
    if (nearestNeighbor(currInstance, instanceSubset) == currInstance.at(0)) {
      ++numCorrect; 
    }
    instanceSubset.clear(); 
    currInstance.clear();
  } 
  //float divided by long double returns as a float value; returning this as a percent value
  return (numCorrect / trainingSet.at(0).size()) * 100; 
} 

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

void backwardSelection(vector<vector<long double>> trainingSet, int numFeatures) { 
  //vector of features with highest accuracy; our final answer
  vector<int> bestFeatures(numFeatures); 

  //initialize the vector of bestFeatures to all possible features (the default rate)
  for (int i = 0; i < numFeatures; ++i) {
    bestFeatures[i] = bestFeatures[i] + (i + 1);
  }

  //vector of features with highest accuracy in the current traversal
  vector<int> currBestFeatures = bestFeatures; 
  float accuracyVal = 0.0;
  float highestAccuracy = 0.0;
    
  for (int i = 1; i < numFeatures; ++i) {
    vector<int> tempHighAccuracyVals;
    float currentHighestA = 0.0; //current highest accuracy for this traversal

    //referenced Project 2_full_briefing.pptx slide 26-27
    for (int j = 0; j <= numFeatures; ++j) {
      vector<vector<long double>> tempSet; 
      vector<int> tempFeatures; 
      tempSet.push_back(trainingSet.at(0)); 
      tempFeatures = currBestFeatures; 
            
      for (int k = 0; k < currBestFeatures.size(); ++k) { 
        //match the temporary set with the training set so they have corresponding rows to iterate through
        tempSet.push_back( trainingSet.at(currBestFeatures.at(k)) ); 
      } 

      vector<int>::iterator it = find(tempFeatures.begin(), tempFeatures.end(), j); 

      //if the iterator value is not found in the vector; assures that you won't compare features with themselves
      if (it != tempFeatures.end()) {
        int index = it - tempFeatures.begin() + 1; 
        tempFeatures.erase(it);
        tempSet.erase(tempSet.begin() + index); 
        cout << "Using { "; 
        for (int i = 0; i < tempFeatures.size(); i++) {
          cout << tempFeatures.at(i) << " ";
        } 

        //function call for cross-validation function to calculate the accuracy
        accuracyVal = crossValidation(tempSet); 
        cout << "} Accuracy: " << accuracyVal << "%" << endl; 

        if (accuracyVal >= currentHighestA) { 
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
    }
    else if (currentHighestA < highestAccuracy) { 
      cout << "Oh Neptune! Accuracy value is decreasing. Checking in case of local maxima." << endl; 
    } 
  } 
  cout << endl << "Searched finished." << endl; 
  cout << "The final best feature subset is: "; 
  for (int i = 0; i < bestFeatures.size(); i++) { 
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
