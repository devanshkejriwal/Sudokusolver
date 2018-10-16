#include <stdbool.h>


struct sudoku;

// the dimension of a Sudoku puzzle
extern const int DIM;



// fill_cell(s,row,col,num) tries to fill num in the cell (row,col).  
// It returns 0 if doing so does not violate any of the row, column, 
// and box constraints.  Otherwise, it returns a negative integer.
// requires: row and col are valid indices. 
//           num is an integer between 1 and 9.
// effects:  mutates *s
// effeciency: o(n) where n is the length of the row/column/box
int fill_cell(struct sudoku *s, int row, int col, int num);

// choices_cell(s,row,col,choices,num_choices) determines all the 
// possible values for the cell (row,col) that do not violate
// any of the row, column, and box constraints.  It mutates choices 
// to contain the possible values and mutates *num_choices to be
// the number of possible values.
// requires: row and col are valid indices.
//           choices is a valid array with length DIM
// effects:  mutates choices and *num_choices
//effeciency: o(n) where n is the length of the sudoku
void choices_cell(const struct sudoku *s, 
                 int row, int col, 
                 int choices[], 
                 int *num_choices);

// solved_puzzle(s) returns true if s has a valid solution to the puzzle, 
// and returns false otherwise.
//efficiency : o(mn) where n is the length of the sudoku and
//             m is the the total possible inputs
bool solved_puzzle(const struct sudoku *s);

// solve(s) solves the Sudoku puzzle by search and backtracking.  
// It mutates the 
// It returns true if s has a valid solution to the puzzle, 
// and returns false otherwise.
// effects: mutates *s
//efficiency: o(n^2) where n is the number of elements in choices 
bool solve(struct sudoku *s);



