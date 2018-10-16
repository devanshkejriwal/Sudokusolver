#include "sudoku.h"
#include "array.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
//static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};

// You need to implement the following four functions.

//row_constraints(s,row,num) checks if the number appears in that 
// particular row.
//efficency: o(n) where n is the length of a row.
bool row_constraints(const struct sudoku *s,int row,int num) {
  int rowindex = row*MAX;
  for(int i = rowindex; i< rowindex+MAX; i++) {
    if(num == s->sol[i])
      return false;
  }
  return true;
}
//col_constraints(s,col,num) checks if the number can be put into 
//the column
//efficency: 0(n) where n is the length of a column
bool col_constraints(const struct sudoku *s,int col,int num) {
  for(int i=col;i< (DIM*DIM)-1;i=i+9) {
    if(num == s->sol[i])
      return false;
  }
  return true;
}

//box_constraints(s,row,col,num) checks if the 
//number can be put into the box
//efficiency: o(n) where n is the length of each row of a box.
bool box_constraints(const struct sudoku *s, int row, int col, int num) {
  int array_index_box=0;
  //searching for starting point of the box
  if(row>=0 && row<=2 && col>=0 && col<=2) {
    array_index_box =0;
  } else if(row>=0 && row<=2 && col>=3 && col<=5) {

    array_index_box =3;

  } else if(row>=0 && row<=2 && col>=6 && col<=8) {

    array_index_box =6;

  } else if(row>=3 && row<=5 && col>=0 && col<=2) {

    array_index_box =27;

  } else if(row>=3 && row<=5 && col>=3 && col<=5) {

    array_index_box = 30;

  } else if(row>=3 && row<=5 && col>=6 && col<=8) {

    array_index_box = 33;

  } else if(row>=6 && row<=8 && col>=0 && col<=2) {

    array_index_box = 54;

  } else if(row>=6 && row<=8 && col>=3 && col<=5) {

    array_index_box = 57;

  } else if(row>=6 && row<=8 && col>=6 && col<=8) {

    array_index_box = 60;
  }

  //checking each row of the box
  //1st row of box;
  for(int i=array_index_box;i < array_index_box + 3; i++) {
    if(num == s->sol[i])
      return false;
  }
  //2nd row of box
  array_index_box = array_index_box+9;
  for(int i=array_index_box;i < array_index_box + 3; i++) {
    if(num == s->sol[i])
      return false;
  }
  //3rd row of box
  array_index_box = array_index_box+9;
  for(int i=array_index_box;i < array_index_box + 3; i++) {
    if(num == s->sol[i])
      return false;
  }
  return true;
}
//valid_fill(s,row,col,num) - checking if we can fill
//a random element in that position
//efficency: o(n).
bool valid_fill(const struct sudoku*s,int row,
                int col,int num) {
  if(row_constraints(s,row,num) &&
     col_constraints(s,col,num) &&
     box_constraints(s,row,col,num)) 
    return true;
  else
    return false;
}


int fill_cell(struct sudoku *s, int row, int col, int num) {
  assert(row>=0 && row <9);
  assert(col>=0 && col <9);
  if(valid_fill(s,row,col,num)) {
    s->sol[row*DIM+col] = num;
    return SUCCESS;
  }
  else
    return ERROR;
}


void choices_cell(const struct sudoku *s, int row, int col, 
                  int choices[], int *num_choices) {
  if(s->puzzle[row*9+col] != 0)
    return;
  int c=0;
  assert(row>=0);
  assert(row<=8);
  assert(col>=0);
  assert(col<=8);
  for(int i =1 ; i<=9 ; i++) {
    if(valid_fill(s,row,col,i) == true) {

      choices[c++] = i;
      *num_choices = *num_choices + 1;
    }
  }
}


bool solved_puzzle(const struct sudoku *s) {
 int row,col;

  for(int i=0; i< DIM*DIM ; i++) {
    if(s->sol[i] == 0) {
      return false;
    }
  }
  int i = 0;
  while(i < DIM*DIM) {
    int j = 1;
    while(j <= 9) {
      row = i/DIM;
      col = i%DIM;
      if(valid_fill(s, row,col, j)) {
        return false;
      
      }
      j++;
    }
     i++;
  }

  return true;
}

bool solve(struct sudoku *s) {
  if(solved_puzzle(s)) {
    return true;
  }
  //finds the next unfilled pos
  int emptycell = -1;
  for(int i =0 ;i<81; i++) {
    if(s->sol[i] == 0) {
      emptycell = i;
      break;
    }
  }
  if(emptycell == -1)
    return true;

  // no possible choice at empty cell position possible
  int totalchoices;
  int a[9] ={0};
  choices_cell(s,emptycell/9,emptycell%9,a,&totalchoices);

  if(totalchoices == 0)
    return false;
  else {
    for(int i =0;i < totalchoices; i++) {
      fill_cell(s,emptycell/9,emptycell%9,a[i]);
      if(solve(s))
        return true; 
      else
        s->sol[emptycell] = EMPTY;
    }
  }
  return false;
}


// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}
