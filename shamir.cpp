#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm> // For next_permutation

using namespace std;
using json = nlohmann::json;

// Function to convert values from a given base to decimal
unsigned long long convertFromBase(int base, const string& value) {
    unsigned long long result = 0;
    int power = 0;
    for (int i = value.size() - 1; i >= 0; i--) {
        char digit = value[i];
        int num = (isdigit(digit)) ? (digit - '0') : (toupper(digit) - 'A' + 10);
        result += num * pow(base, power);
        power++;
    }
    return result;
}

// Function to transform JSON-like input into a vector of points
vector<pair<int, unsigned long long>> parsePoints(map<int, pair<int, string>>& data) {
    vector<pair<int, unsigned long long>> points;
    
    for (auto& entry : data) {
        int x = entry.first;
        int base = entry.second.first;
        string value = entry.second.second;
        unsigned long long y = convertFromBase(base, value);
        points.push_back({x, y});
    }
    
    return points;
}

// Function to compute the polynomial constant term using Lagrange interpolation
double computeLagrangeConstant(const vector<pair<int, unsigned long long>>& points) {
    int count = points.size(); // Number of points
    double constantTerm = 0.0;
    
    for (int i = 0; i < count; i++) {
        double li = 1.0;
        unsigned long long yi = points[i].second;
        
        for (int j = 0; j < count; j++) {
            if (i != j) {
                li *= points[j].first / (double)(points[j].first - points[i].first);
            }
        }
        
        constantTerm += yi * li;
    }
    
    return constantTerm;
}

// Function to verify if a given point fits the interpolated polynomial
bool validatePoint(const vector<pair<int, unsigned long long>>& points, int x, unsigned long long y, double constantTerm) {
    double interpolatedY = 0.0;
    for (int i = 0; i < points.size(); i++) {
        double li = 1.0;
        for (int j = 0; j < points.size(); j++) {
            if (i != j) {
                li *= (x - points[j].first) / (double)(points[i].first - points[j].first);
            }
        }
        interpolatedY += points[i].second * li;
    }

    // Debug output
    cout << "Testing point (" << x << ", " << y << "): ";
    cout << "Interpolated Y = " << interpolatedY << ", Actual Y = " << y << ", Difference = " << fabs(interpolatedY - y) << endl;

    return fabs(interpolatedY - y) < 1e-6; 
}

int main() {
    ifstream inputFile("input.json");
    json jsonData;
    inputFile >> jsonData;

    int n = jsonData["keys"]["n"];
    int k = jsonData["keys"]["k"];

    map<int, pair<int, string>> rootData;
    for (auto& el : jsonData.items()) {
        if (el.key() == "keys") continue; 
        int x = stoi(el.key());            
        int base = stoi(el.value()["base"].get<string>());
        string encodedY = el.value()["value"];
        rootData[x] = {base, encodedY};
    }

    vector<pair<int, unsigned long long>> allPoints = parsePoints(rootData);

    vector<int> indices(n, 0);
    fill(indices.begin(), indices.begin() + k, 1); 

    bool solutionFound = false;
    vector<int> incorrectPoints;

    do {
        vector<pair<int, unsigned long long>> selectedPoints;
        vector<pair<int, unsigned long long>> remainingPoints;

        for (int i = 0; i < n; i++) {
            if (indices[i] == 1) {
                selectedPoints.push_back(allPoints[i]);
            } else {
                remainingPoints.push_back(allPoints[i]);
            }
        }

        double constantTerm = computeLagrangeConstant(selectedPoints);

        vector<int> currentIncorrectPoints;
        for (auto& point : remainingPoints) {
            if (!validatePoint(selectedPoints, point.first, point.second, constantTerm)) {
                currentIncorrectPoints.push_back(point.first);
            }
        }

        if (currentIncorrectPoints.size() <= 3) {  
            solutionFound = true;
            incorrectPoints = currentIncorrectPoints;
            break;
        }

    } while (prev_permutation(indices.begin(), indices.end())); 

    if (solutionFound) {
        cout << "The incorrect roots as per the k value are: ";
        for (int root : incorrectPoints) {
            cout << root << " ";
        }
        cout << endl;
    } else {
        cout << "No solution found!" << endl;
    }

    return 0;
}
