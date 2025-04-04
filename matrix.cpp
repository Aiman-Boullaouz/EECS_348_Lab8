#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <utility>

using namespace std;

bool loadMatricesFromFile(const string& filename, int& n, vector<vector<int>>& a, vector<vector<int>>& b) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        return false;
    }

    file >> n;
    string dummy;
    getline(file, dummy); // Consume the newline after n

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

void printMatrix(const vector<vector<int>>& matrix) {
    if (matrix.empty()) {
        cout << "Matrix is empty." << endl;
        return;
    }
    int n = matrix.size();
    int max_width = 0;
    for (const auto& row : matrix) {
        for (int val : row) {
            int width = to_string(val).length();
            if (width > max_width) {
                max_width = width;
            }
        }
    }
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << setw(max_width + 1) << val;
        }
        cout << endl;
    }
}

void addAndDisplay(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();
    vector<vector<int>> result(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    cout << "Sum of matrices:" << endl;
    printMatrix(result);
}

void multiplyAndDisplay(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();
    vector<vector<int>> result(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    cout << "Product of matrices:" << endl;
    printMatrix(result);
}

pair<int, int> sumDiagonals(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    int mainSum = 0, secondarySum = 0;
    for (int i = 0; i < n; ++i) {
        mainSum += matrix[i][i];
        secondarySum += matrix[i][n - 1 - i];
    }
    return {mainSum, secondarySum};
}

void displayDiagonals(const vector<vector<int>>& matrix, const string& name) {
    auto sums = sumDiagonals(matrix);
    cout << "Sum of main diagonal of " << name << ": " << sums.first << endl;
    cout << "Sum of secondary diagonal of " << name << ": " << sums.second << endl;
}

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

void swapColumnsAndDisplay(vector<vector<int>> matrix, int col1, int col2, const string& name) {
    int n = matrix.size();
    if (col1 < 0 || col1 >= n || col2 < 0 || col2 >= n) {
        cout << "Invalid column indices." << endl;
        return;
    }
    for (int i = 0; i < n; ++i) {
        swap(matrix[i][col1], matrix[i][col2]);
    }
    cout << "Matrix " << name << " after swapping columns " << col1 << " and " << col2 << ":" << endl;
    printMatrix(matrix);
}

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

    if (!loadMatricesFromFile(filename, n, a, b)) {
        return 1;
    }

    cout << "Matrix A:" << endl;
    printMatrix(a);
    cout << "\nMatrix B:" << endl;
    printMatrix(b);

    addAndDisplay(a, b);
    multiplyAndDisplay(a, b);

    displayDiagonals(a, "A");
    displayDiagonals(b, "B");

    swapRowsAndDisplay(a, 0, 3, "A");
    swapColumnsAndDisplay(b, 0, 3, "B");
    updateAndDisplay(a, 0, 0, 99, "A");

    return 0;
}