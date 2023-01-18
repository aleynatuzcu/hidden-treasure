#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

//splitting dimension argument
vector<int> dim(const string &arg) {
    vector<int> dim;
    dim.push_back(stoi(arg.substr(0, arg.find('x'))));
    dim.push_back(stoi(arg.substr(arg.find('x') + 1)));

    return dim;
}

//calculating dot product of two matrices
int dotProduct(vector<vector<int> > key, vector<vector<int> > map, int matrixSize) {
    int dotProduct = 0;

    int m = floor(matrixSize / 2); //floor division

    int sRow = m;
    int sColumn = m;
    for (int i = -m; i <= m; ++i) {
        for (int j = -m; j <= m; ++j) {
            dotProduct += key[sRow + i][sColumn + j] * map[sRow + i][sColumn + j];
        }
    }
    return dotProduct;
}

//reading txt file and converting to 2D matrices
vector<vector<int> > readMatrix(const string &path) {
    vector<vector<int> > grid;
    fstream keyMatrix(path);

    if (keyMatrix.is_open()) {
        string line;
        while (getline(keyMatrix, line)) {
            string token;
            istringstream ss(line);

            vector<int> row;

            while (getline(ss, token, ' '))
                row.push_back(stoi(token));

            grid.push_back(row);
        }
        keyMatrix.close();
    }
    return grid;
}

//taking submatrix from given index by given size
vector<vector<int> > subMatrix(vector<vector<int> > mapMatrix, int size, int rowIndex, int colIndex) {
    vector<vector<int> > sub;

    int m = floor(size / 2);

    for (int i = -m; i <= m; ++i) {
        vector<int> row;
        for (int j = -m; j <= m; ++j) {
            row.push_back(mapMatrix[rowIndex + i][colIndex + j]);
        }
        sub.push_back(row);
    }

    return sub;
}

//func for taking mod5 of dotProduct
int modFive(int dotProduct) {
    if (dotProduct >= 0) {
        return dotProduct % 5;
    }
    return (dotProduct % 5) + 5;
}

//checking if index is out of bound
bool
isOutside(const vector<vector<int> > &map, int rowIndex, int colIndex, int size, int nRow, int nCol, int direction) {

    if (direction == 0) {
        return false;
    }

    if (direction == 1) {
        if ((rowIndex - size) < 0) { return true; }
        else return false;
    }

    if (direction == 2) {
        if ((rowIndex + size) > nRow) { return true; }
        else return false;
    }

    if (direction == 3) {
        if ((colIndex + size) > nCol) { return true; }
        else return false;
    }

    if (direction == 4) {
        if ((colIndex - size) < 0) { return true; }
        else return false;
    }

    return false;
}

//moving right function
void moveRight(int &colIndex, int keySize) {
    colIndex = colIndex + keySize;
}

//moving left function
void moveLeft(int &colIndex, int keySize) {
    colIndex = colIndex - keySize;
}

//moving up function
void moveUp(int &rowIndex, int keySize) {
    rowIndex = rowIndex - keySize;
}

//moving down function
void moveDown(int &rowIndex, int keySize) {
    rowIndex = rowIndex + keySize;
}

int main(int argc, char* argv[]) {

    ofstream output;
    string filename(argv[5]);
    output.open(filename);

    string a(argv[1]);
    vector<int> dimension = dim(a);

    int lenMapRow = dimension[0];
    int lenMapCol = dimension[1];

    int keySize = stoi(argv[2]);

    int rowIndex = floor(keySize / 2);
    int colIndex = floor(keySize / 2);

    string pathMap(argv[3]);
    string pathKey(argv[4]);

    vector<vector<int> > map = readMatrix(pathMap);
    vector<vector<int> > key = readMatrix(pathKey);


    vector<vector<int> > submatrix = subMatrix(map, keySize, rowIndex, colIndex);
    int dotproduct = dotProduct(submatrix, key, keySize);
    int mod = modFive(dotproduct);

    bool outside = isOutside(map, rowIndex, colIndex, keySize, lenMapRow, lenMapCol, mod);

    while (true) {
        if (!outside) { //if it is not outside, go to given direction

            output<< rowIndex << "," << colIndex << ":" << dotproduct <<endl;
            if (mod == 0) {
                break;
            }
            if (mod == 1) { //up
                moveUp(rowIndex, keySize);
            }
            if (mod == 2) { //down
                moveDown(rowIndex, keySize);
            }
            if (mod == 3) { //right
                moveRight(colIndex, keySize);
            }
            if (mod == 4) { //left
                moveLeft(colIndex, keySize);
            }

        }
        if (outside) { //if it is outside, go to opposite of given direction

            output<< rowIndex << "," << colIndex << ":" << dotproduct <<endl;
            if (mod == 1) { // if up, go down
                moveDown(rowIndex, keySize);
            }
            if (mod == 2) { //if down, go up
                moveUp(rowIndex, keySize);
            }
            if (mod == 3) { // if right, go left
                moveLeft(colIndex, keySize);
            }
            if (mod == 4) { //if left, go right
                moveRight(colIndex, keySize);
            }
        }

        submatrix = subMatrix(map, keySize, rowIndex, colIndex);
        dotproduct = dotProduct(submatrix, key, keySize);
        mod = modFive(dotproduct);

        outside = isOutside(map, rowIndex, colIndex, keySize, lenMapRow, lenMapCol, mod);
    }
    output.close();
}
