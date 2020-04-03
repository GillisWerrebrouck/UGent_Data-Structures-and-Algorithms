#include <iostream>
#include <string>
#include "Sudoku.h"

void solveAll() {
    int sum = 0;
    for(int i = 1; i <= 50; i++) {
        Sudoku s("./sudokus/" + std::to_string(i) + ".txt");
        s.solve();
        
        sum += s.sumOfThreeFirstCells();
        if(sum == -1) {
            break;
        }
    }

    if(sum == -1) {
        std::cout << "something went wrong when solving all sudokus" << std::endl;
    } else {
        std::cout << "the sum of all 3-digit numbers in the upper left corner of each sudoku: " << sum << std::endl;
    }
}

/**
 * backtracking algorithm to solve a sudoku:
 * The algorithm first looks for an unassigned cell, if there is one. If found it will try every possible value (1-9) for a cell. 
 * For every value it will check if the value is valid by checking if that value doesn't aleady occurs in its row, column and subgrid. 
 * If a value is valid then it is assigned to that cell. If the value is not valid, the next possible value will be checked. 
 * If all values seem to be invalid, the recursion will return false and the previous step will be undone. The previous step will 
 * then try the next possible value until a valid value is found. The method will return back for as long as all possible values 
 * for a cell are invalid. This bruteforce approach is good if a lot of final values can be found fast. This is the case if a lot 
 * of hints (good initial given values to start with) are provided. In the worst case it could iterate all the way to the end and 
 * then find out at the end that there is no valid value for that cell. It then could, again worst case, iterate back all the way 
 * to the beginning because that value could've been wrong all along.
 * 
 * Checking if a given sudoku is solved is as simple as checking if each row, column and subgrid has no duplicates and all cells have 
 * a value assigned. This is accomplished by using a boolean array to check for duplicates in each row, column and subgrid. The boolean 
 * array gets reset after checking each row, column or subgrid. Sorting each row, column or subgrid would be a possibily too but not a good 
 * one. It would require to make a copy first [ O(n) ] and then sort it [ O(n*lg(n)) ]. Every row, column and subgrid only has 9 values so 
 * using a boolean array and checking for duplicates is O(n) while copying and sorting would be O(n + n*lg(n)). But again because the arrays 
 * only have 9 elements it would only have a slight unnoticeable difference.
 * 
 * Additional explanation is provided at each step of the methods.
 **/

int main() {
    Sudoku s("./1.txt");
    std::cout << "unsolved sudoku:" << std::endl << s << std::endl;
    s.solve();
    if(s.isSolved()) {
        std::cout << "solved sudoku:" << std::endl << s << std::endl;
    } else {
        std::cout << "sudoku could not be solved:" << std::endl << s << std::endl;
    }

    std::cout << "solving all sudokus:" << std::endl;
    solveAll();
}
