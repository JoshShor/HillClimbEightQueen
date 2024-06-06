#include <iostream>
#include <vector>
#include <ctime> // Need ctime to do random number generation based on the computer clock

using namespace std;

/* Generate random solution with queens placed randomly */
vector<int> randSoln() { 
    vector<int> solution(8); // Vector to store the solution with 8 elements initialized to 0
    
    for (int i = 0; i < 8; ++i) {
        solution[i] = rand() % 8; // Assign a random row (position) between 0 and 7 inclusive
    }
    
    return solution;
}

/* Count number of conflicts */
int conflicts(const vector<int>& qPosition) {
    int numConflicts = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = i + 1; j < 8; ++j) {
            // Check if two queens are in the same row or in diagonals
            if (qPosition[i] == qPosition[j] || abs(qPosition[i] - qPosition[j]) == abs(i - j)) {
                numConflicts++;
            }
        }
    }
    return numConflicts;
}

/* Print Chess Board */
void printSolution(const vector<int>& solution) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            // If the queen is placed at the current position, print 'Q', otherwise print '+'
            if (solution[col] == row) {
                cout << "Q ";
            } else {
                cout << "+ ";
            }
        }
        cout << endl; // Print the row number for each queen
    }
    cout << endl;
}

/* Hill climbing algorithm */
vector<int> hillClimbAlgo() {
    srand(time(0)); // Seed for random number generation
    
    vector<int> currentSoln = randSoln(); // Initialize the current solution with a random placement of queens
    int queenConflict = conflicts(currentSoln); // Calculate the number of conflicts in the current solution
    int nAttackPair = 28 - queenConflict; // Calculate the number of non-attacking pairs
    int prevConflicts = queenConflict; // Track the previous number of conflicts
    int noImprovementCount = 0; // Track the number of iterations without improvement

    // Continue until a solution with zero conflicts is found or a termination condition is met
    while (nAttackPair < 28 && noImprovementCount < 1000) {
        cout << "Score (number of non-attacking pairs): " << nAttackPair << endl;
        cout << "Current State of Board:" << endl;
        printSolution(currentSoln);
        
        vector<int> nextSoln = currentSoln; // Initialize the next solution as a copy of the current solution
        int bestSoln = queenConflict; // Track the number of conflicts in the best solution

        // Iterate through all possible moves of each queen
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (nextSoln[i] != j) { // Only consider moves to a different row
                    nextSoln[i] = j;
                    int newConflicts = conflicts(nextSoln); // Calculate the number of conflicts in the next solution
                    
                    // If the new solution has fewer conflicts, update the best solution
                    if (newConflicts < bestSoln) {
                        bestSoln = newConflicts;
                        currentSoln = nextSoln;
                    }
                }
                nextSoln[i] = currentSoln[i]; // Reset the row position for the next iteration
            }
        }

        nAttackPair = 28 - bestSoln; // Update the number of non-attacking pairs

        // If no improvement is found
        if (bestSoln >= queenConflict) {
            noImprovementCount++;
            
            if (prevConflicts == queenConflict) {
                cout << "Stuck at a plateau.\nRestart search with new set of random positions for queens." << endl;
            } else if (noImprovementCount > 1 && queenConflict > prevConflicts) {
                cout << "Stuck at a ridge.\nRestart search with new set of random positions for queens." << endl;
            } else {
                cout << "Stuck at a local maximum.\nRestart search with new set of random positions for queens." << endl;
            }
            
            currentSoln = randSoln(); // Restart with a new random solution
            queenConflict = conflicts(currentSoln);
            prevConflicts = queenConflict;
            nAttackPair = 28 - queenConflict;
        } else {
            noImprovementCount = 0;
            prevConflicts = queenConflict;
            queenConflict = bestSoln;
        }
    }

    return currentSoln; // Return the solution with zero conflicts
}

int main() {
    vector<int> solution = hillClimbAlgo();
    
    // Print the solution
    cout << "Final Solution:" << endl;
    printSolution(solution);

    return 0;
}
