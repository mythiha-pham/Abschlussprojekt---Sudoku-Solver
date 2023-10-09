/* Projektarbeit C-Kurs, Blockkurs WiSe 2023
 * Kursteilnehmer:
 *    Thi Ha My Pham, Matrikelnummer 2230890
 *
 * Projektname:
 *    Sudoku-Solver with Backtracking: make a guess, check row, column, and subgrid constraints
 *      - if any constraint is violated, backtrack and choose a different number
 *      - if all constraints are met, update the cell's value and continue 
 * Compileraufruf
 *    g++ solver.cpp
 *    ./a.out
 */

#include <iostream>
#include <vector>

using namespace std;

const int N = 9; // Default number of the rows and columns in the Sudoku grid

// Print the Sudoku grid into the console
void printSudokuGrid(int grid[N][N]) {
    for( int row = 0; row < N; row++) {
        if (row % 3 == 0 && row > 0) {
            // Print a horizontal line to separate subgrids
            cout << "------+-------+------\n";
        }

        for (int col = 0; col < N; col++) {
            if (col % 3 == 0 && col > 0) {
                // Print a vertical line to separate subgrids
                cout << "| ";
            }
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

// Check if a number can be placed in a cell
bool isValid(int grid[N][N], int row, int col, int n) {
    // Check if a cell is empty (represented by 0)
    if(grid[row][col] != 0) {
        return false;
    }

    bool status = true;

    // Calculate the top-left corner coordinates of the 3x3 subgrid containing the cell in (row, col)
    int gridX = (row / 3) * 3;
    int gridY = (col / 3) * 3;

    // Go through all rows and columns
    // If the number exists in a row, column or a subgrid where the cell (row, col) is located, 
    // break the loop and not check further.
    for(int i = 0; i < N; i++) {
        // Check row constraint
        if(grid[row][i] == n) {
            status = false;
            break;
        }

        // Check column constraint
        if(grid[i][col] == n) {
            status = false;
            break;
        }
        
        // Check 3x3 subgrid constraint
        if(grid[gridX + i / 3][gridY + i % 3] == n) {
            status = false;
            break;
        }
    }
    return status;
}

// Find empty cells
vector<int> findPlaceables (int grid[N][N], int row, int col) {
    // Create a vector to store the valid values that can be placed in the grid
    vector<int> placeables = {};
    for (int i = 0; i <= N; i++) {
        // Check if the value 'i' can be placed in the grid at the given 'row' and 'col'
        if (isValid(grid, row, col, i)) {
            // If it's a valid placement, add 'i' to the placeables vector
            placeables.push_back(i);
        }
    }
    // Return the vector containing valid placements
    return placeables;
}

// Ensure that the original grid remains intact by creating a duplicate of it before making any changes
void copyGrid(int grid[N][N], int gridCpy[N][N]) {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            // Copy the value at grid[x][y] to gridCpy[x][y]
            gridCpy[x][y] = grid[x][y];
        }
    }
}

// Find the next empty cell
void nextEmpty(int grid[N][N], int row, int col, int &nextRow, int &nextCol) {
    int nextIndex = N * N + 1; // Ensure that there are no more empty cells
    // row * N + col calculates the index of the current cell,
    // and adding 1 moves to the next cell in the same row
    for(int i = row * N + col + 1; i < N * N; i++) {
        // Check if the cell at the current index is empty
        if(grid[i / N][i % N] == 0) { 
            nextIndex = i;
            break;
        }
    }
    nextRow = nextIndex / N;
    nextCol = nextIndex % N;
}

// Recursive function to explore all the possibilities
bool solveSudoku(int grid[N][N], int row, int col) {
    if(row >= N) {
        return true; // Done computing all the cells. Sudoku solved.
    }

    //  Check if the cell is filled. If so, skip it and compute the next cell. 
    if(grid[row][col] != 0) {
        int nextRow, nextCol;
        nextEmpty(grid, row, col, nextRow, nextCol);
        return solveSudoku(grid, nextRow, nextCol);
    }

    // Find all placeable values
    vector<int> placeables = findPlaceables(grid, row, col);

    if(placeables.size() == 0) {
        return false;
    }

    bool status = false;

    // Backtracking: Create a new copy of the array and check whether putting a particular placeable value into an array cell is solvable or not.
    // If it is solvable, call the function again for the next cell until all the cells are explored.
    for(int i = 0; i < placeables.size(); i++) {
        int n = placeables[i];
        // Create a copy of the original grid
        int gridCpy[N][N];
        copyGrid(grid, gridCpy);

        // Place the value 'n' in the current cell (row, col) of the copied grid
        gridCpy[row][col] = n;

        // Initialize variables to store the next empty cell's row and column
        int nextRow, nextCol;
        // Find the coordinates of the next empty cell in the copied grid
        nextEmpty(gridCpy, row, col, nextRow, nextCol);

        // Recursively attempt to solve the Sudoku puzzle starting from the current configuration
        if(solveSudoku(gridCpy, nextRow, nextCol)) {
            // If a solution is found, copy the solution back to the original grid
            copyGrid(gridCpy, grid);
            // Set the 'status' to true to indicate a solution was found
            status = true;
            break;
        }
    }
    return status;
}

int main() {

    int grid[N][N];

    cout << "Enter the Sudoku grid (without space, use 0 for empty cells):" << endl;

    for (int i = 0; i < N; i++) {
        string input;
        cout << "Enter numbers in row " << i + 1 << ": ";
        cin >> input;
        for (int j = 0; j < N; j++) {
            grid[i][j] = input[j] - '0'; // Convert the character digit to an integer
        }
    }

    if (solveSudoku(grid, 0, 0)) {
        cout << "Solved Sudoku:" << endl;
	    printSudokuGrid(grid);
    } else {
        cout << "No solution exists!" << endl;
    }

    return 0;
}