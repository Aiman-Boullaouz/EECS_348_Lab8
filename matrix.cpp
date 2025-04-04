#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <utility>
#include <limits>
#include <cctype>

using namespace std;

// Loads two n x n matrices A and B from a file
bool loadMatricesFromFile(const string& filename, int& n, vector<vector<int>>& a, vector<vector<int>>& b) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        return false;
    }

    file >> n; // Read matrix size
    string dummy;
    getline(file, dummy); // Consume leftover newline

    // Read matrix A
    a.resize(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        string line;
        if (!getline(file, line)) {
            cerr << "Error reading matrix A row " << i << "." << endl;
            return false;
        }
        istringstream iss(line);
        for (int j = 0; j < n; ++j) {
            if (!(iss >> a[i][j])) {
                cerr << "Error reading matrix A element at row " << i << ", column " << j << "." << endl;
                return false;
            }
        }
    }

    // Read matrix B
    b.resize(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        string line;
        if (!getline(file, line)) {
            cerr << "Error reading matrix B row " << i << "." << endl;
            return false;
        }
        istringstream iss(line);
        for (int j = 0; j < n; ++j) {
            if (!(iss >> b[i][j])) {
                cerr << "Error reading matrix B element at row " << i << ", column " << j << "." << endl;
                return false;
            }
        }
    }

    return true;
}

// Prints a matrix with aligned formatting
void printMatrix(const vector<vector<int>>& matrix) {
    if (matrix.empty()) {
        cout << "Matrix is empty." << endl;
        return;
    }

    int max_width = 0;
    // Find max width for formatting
    for (const auto& row : matrix) {
        for (int val : row) {
            int width = to_string(val).length();
            if (width > max_width) {
                max_width = width;
            }
        }
    }

    // Print each row with proper spacing
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(max_width + 1) << val;
        }
        cout << endl;
    }
}

// Adds matrices A and B and prints the result
void addAndDisplay(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();
    vector<vector<int>> result(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            result[i][j] = a[i][j] + b[i][j];

    cout << "Sum of matrices:" << endl;
    printMatrix(result);
}

// Multiplies matrices A and B and prints the result
void multiplyAndDisplay(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();
    vector<vector<int>> result(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                result[i][j] += a[i][k] * b[k][j];

    cout << "Product of matrices:" << endl;
    printMatrix(result);
}

// Returns the sums of the main and secondary diagonals
pair<int, int> sumDiagonals(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int mainSum = 0, secondarySum = 0;

    for (int i = 0; i < n; ++i) {
        mainSum += matrix[i][i];             // Main diagonal: top-left to bottom-right
        secondarySum += matrix[i][n - 1 - i]; // Secondary diagonal: top-right to bottom-left
    }

    return {mainSum, secondarySum};
}

// Displays the diagonal sums of a matrix
void displayDiagonals(const vector<vector<int>>& matrix, const string& name) {
    auto sums = sumDiagonals(matrix);
    cout << "Sum of main diagonal of " << name << ": " << sums.first << endl;
    cout << "Sum of secondary diagonal of " << name << ": " << sums.second << endl;
}

// Swaps two rows of a matrix and displays the result
void swapRowsAndDisplay(vector<vector<int>> matrix, int row1, int row2, const string& name) {
    int n = matrix.size();
    if (row1 < 0 || row1 >= n || row2 < 0 || row2 >= n) {
        cout << "Invalid row indices." << endl;
        return;
    }
    swap(matrix[row1], matrix[row2]);
    cout << "Matrix " << name << " after swapping rows " << row1 << " and " << row2 << ":" << endl;
    printMatrix(matrix);
}

// Swaps two columns of a matrix and displays the result
void swapColumnsAndDisplay(vector<vector<int>> matrix, int col1, int col2, const string& name) {
    int n = matrix.size();
    if (col1 < 0 || col1 >= n || col2 < 0 || col2 >= n) {
        cout << "Invalid column indices." << endl;
        return;
    }

    for (int i = 0; i < n; ++i)
        swap(matrix[i][col1], matrix[i][col2]);

    cout << "Matrix " << name << " after swapping columns " << col1 << " and " << col2 << ":" << endl;
    printMatrix(matrix);
}

// Updates a matrix element and displays the result
void updateAndDisplay(vector<vector<int>> matrix, int row, int col, int newVal, const string& name) {
    int n = matrix.size();
    if (row < 0 || row >= n || col < 0 || col >= n) {
        cout << "Invalid indices." << endl;
        return;
    }

    matrix[row][col] = newVal;
    cout << "Matrix " << name << " after updating element at (" << row << "," << col << ") to " << newVal << ":" << endl;
    printMatrix(matrix);
}

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    int n;
    vector<vector<int>> a, b;

    // Load matrices A and B
    if (!loadMatricesFromFile(filename, n, a, b)) {
        return 1;
    }

    // Display both matrices
    cout << "Matrix A:" << endl;
    printMatrix(a);
    cout << "\nMatrix B:" << endl;
    printMatrix(b);

    // Perform addition and multiplication
    addAndDisplay(a, b);
    multiplyAndDisplay(a, b);

    // Display diagonal sums
    displayDiagonals(a, "A");
    displayDiagonals(b, "B");

    // Function to select which matrix to operate on
    auto getValidMatrix = [&](string& name) -> vector<vector<int>>* {
        char choice;
        while (true) {
            cout << "Select matrix (A/B) to operate on: ";
            cin >> choice;
            choice = toupper(choice);

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again.\n";
            }
            else if (choice == 'A') {
                name = "A";
                return &a;
            }
            else if (choice == 'B') {
                name = "B";
                return &b;
            }
            else {
                cout << "Invalid matrix choice. Please enter A or B.\n";
            }
        }
    };

    // Prompt and validate row swap input
    {
        cout << "Swap Rows\n";
        string name;
        auto matrix = getValidMatrix(name);

        int row1, row2;
        while (true) {
            cout << "Enter two row indices to swap (0-" << n-1 << ") (Example input: '0 2'): ";
            string input;
            cin.ignore();
            getline(cin, input);
            istringstream iss(input);

            if (iss >> row1 >> row2) {
                char remaining;
                if (iss >> remaining) {
                    cout << "Invalid input! Only two numbers allowed.\n";
                }
                else if (row1 >= 0 && row1 < n && row2 >= 0 && row2 < n) {
                    break;
                }
                else {
                    cout << "Indices out of range! Valid range: 0-" << n-1 << "\n";
                }
            } else {
                cout << "Invalid input! Please enter TWO numbers.\n";
            }
        }

        swapRowsAndDisplay(*matrix, row1, row2, name);
    }

    // Prompt and validate column swap input
    {
        cout << "Swap Columns\n";
        string name;
        auto matrix = getValidMatrix(name);

        int col1, col2;
        while (true) {
            cout << "Enter two column indices to swap (0-" << n-1 << ") (Example input: '0 2'): ";
            string input;
            cin.ignore();
            getline(cin, input);
            istringstream iss(input);

            if (iss >> col1 >> col2) {
                char remaining;
                if (iss >> remaining) {
                    cout << "Invalid input! Only two numbers allowed.\n";
                }
                else if (col1 >= 0 && col1 < n && col2 >= 0 && col2 < n) {
                    break;
                }
                else {
                    cout << "Indices out of range! Valid range: 0-" << n-1 << "\n";
                }
            } else {
                cout << "Invalid input! Please enter TWO numbers.\n";
            }
        }

        swapColumnsAndDisplay(*matrix, col1, col2, name);
    }

    // Prompt and validate element update input
    {
        cout << "Update Element\n";
        string name;
        auto matrix = getValidMatrix(name);

        int row, col, newVal;
        while (true) {
            cout << "Enter row index (0-" << n-1 << ") (Example input: '0'): ";
            if (cin >> row && row >= 0 && row < n) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid row! Please enter 0-" << n-1 << "\n";
        }

        while (true) {
            cout << "Enter column index (0-" << n-1 << ") (Example input: '0'): ";
            if (cin >> col && col >= 0 && col < n) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid column! Please enter 0-" << n-1 << "\n";
        }

        while (true) {
            cout << "Enter new integer value: ";
            if (cin >> newVal) break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid value! Please enter an integer.\n";
        }

        updateAndDisplay(*matrix, row, col, newVal, name);
    }

    return 0;
}
