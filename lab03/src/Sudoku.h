#ifndef SUDOKU_H
#define SUDOKU_H

#include <fstream>
#include <iostream>

// a coordinate represents a cell in a sudoku grid
struct Coordinate_t {
  int row, col;
};
typedef struct Coordinate_t Coordinate_t;

class Sudoku{
public:
    Sudoku(const std::string filename);
    bool isSolved() const;
    const Coordinate_t * findNextUnassignedCell() const;
    bool isValidInRow(int row, int value) const;
    bool isValidInColumn(int col, int value) const;
    bool isValidInSubgrid(const Coordinate_t * cell, int value) const;
    bool isValid(const Coordinate_t * c, int value) const;
    bool solve();
    int sumOfThreeFirstCells() const;
    friend std::ostream& operator<< (std::ostream &out, const Sudoku &s);
private:
    int grid[9][9];
};

std::ostream& operator<< (std::ostream &out, const Sudoku &s){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            out << s.grid[i][j] << " " ;

            if ((j+1) % 3 == 0 && j < 8)
                out << "| ";
        }
        out << std::endl;
        if ((i+1) % 3 == 0 && i < 8){
            out << "------+-------+------" << std::endl;
        }
    }
    return out;
}

Sudoku::Sudoku(const std::string filename){
    std::ifstream infile(filename);
    if (infile){
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                infile >> grid[i][j];
            }
        }
    }
}

// a sudoku is solved if no suplicates occur in any row, column or subgrid
bool Sudoku::isSolved() const {
    // an array of type boolean keeps track of what value has been seen in a row, column or subgrid
    bool check[9]{false};

    // check duplicates in rows
    for(int r = 0; r < 9; r++) {
        for(int c = 0; c < 9; c++) {
            // the sudoku is not solved if the value has already been seen in the current row
            if(check[grid[r][c]-1])
                return false;
            // set value as seen
            check[grid[r][c]-1] = true;
        }

        // reset array of seen values for next row
        for(int i = 0; i < 9; i++) {
            check[i] = false;
        }
    }

    // check duplicates in cols
    for(int c = 0; c < 9; c++) {
        for(int r = 0; r < 9; r++) {
            // the sudoku is not solved if the value has already been seen in the current column
            if(check[grid[r][c]-1])
                return false;
            // set value as seen
            check[grid[r][c]-1] = true;
        }

        // reset array of seen values for next column
        for(int i = 0; i < 9; i++) {
            check[i] = false;
        }
    }

    // check duplicates in subgrids
    // iterate over each 3x3 subgrid
    for(int subgridrow = 0; subgridrow < 9; subgridrow += 3) {
        for(int subgridcol = 0; subgridcol < 9; subgridcol += 3) {
            // iterate over each cell in the current subgrid
            for(int r = subgridrow; r < subgridrow+3; r++) {
                for(int c = subgridcol; c < subgridcol+3; c++) {
                    // the sudoku is not solved if the value has already been seen in the current subgrid
                    if(check[grid[r][c]-1])
                        return false;
                    // set value as seen
                    check[grid[r][c]-1] = true;
                }
            }

            // reset array of seen values for next subgrid
            for(int i = 0; i < 9; i++) {
                check[i] = false;
            }
        }
    }

    return true;
}

// find the first occurance of an unassigned cell (value 0) and return its coordinates
const Coordinate_t * Sudoku::findNextUnassignedCell() const {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if(grid[r][c] == 0) {
                return new Coordinate_t{r, c};
            }
        }
    }
    // if no unassigned cell can be found
    return new Coordinate_t{-1, -1};
}

// check if a value is valid for a row
bool Sudoku::isValidInRow(int row, int value) const {
    for(int c = 0; c < 9; c++) {
        if(grid[row][c] == value) {
            return false;
        }
    }
    return true;
}

// check if a value is valid for a column
bool Sudoku::isValidInColumn(int col, int value) const {
    for(int r = 0; r < 9; r++) {
        if(grid[r][col] == value) {
            return false;
        }
    }
    return true;
}

// check if a value is valid for a subgrid
bool Sudoku::isValidInSubgrid(const Coordinate_t * cell, int value) const {
    // get the start row and start column of the subgrid based on the given cell coordinates
    int startRow = cell->row - cell->row % 3;
    int startCol = cell->col - cell->col % 3;

    for(int r = startRow; r < startRow+3; r++) {
        for(int c = startCol; c < startCol+3; c++) {
            if(grid[r][c] == value) {
                return false;
            }
        }
    }
    return true;
}

// check if a value is valid for the coordinates of a given cell
bool Sudoku::isValid(const Coordinate_t * coordinate, int value) const {
    return (
        isValidInRow(coordinate->row, value) &&
        isValidInColumn(coordinate->col, value) &&
        isValidInSubgrid(coordinate, value)
    );
}

bool Sudoku::solve() {
    const Coordinate_t * coordinate = findNextUnassignedCell();

    // if no more unassigned cells then the sudoku is solved
    if(coordinate->row == -1 && coordinate->col == -1) {
        return true;
    }

    // try every possible value (1-9)
    for(int value = 1; value <= 9; value++) {
        // check if the value is valid for the current cell, if not try the another value
        if(!isValid(coordinate, value)) {
            continue;
        }
        
        // assign the value to the cell
        grid[coordinate->row][coordinate->col] = value;

        // recursively solve the sudoku
        if(solve()) {
            return true;
        }
        
        // unassign the cell if this step (value) seems to be wrong for future cells
        grid[coordinate->row][coordinate->col] = 0;
    }

    // backtrack to a previously assigned cell if no value is valid for the current cell
    return false;
}

// return the value for the 3-digit number in the upper left corner if the sudoku is solved, otherwise return -1
int Sudoku::sumOfThreeFirstCells() const {
    if(isSolved()) {
        return grid[0][0]*100 + grid[0][1]*10 + grid[0][2];
    } else {
        return -1;
    }
}

#endif
