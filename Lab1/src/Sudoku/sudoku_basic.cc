#include <assert.h>
#include <stdio.h>

#include <algorithm>

#include "sudoku.h"

//int board[N];
//int spaces[N];
//int nspaces;
//int (*chess)[COL] = (int (*)[COL])board;

static void find_spaces(Datas* d_ele)
{
  d_ele->nspaces = 0;
  for (int cell = 0; cell < N; ++cell) {
    if (d_ele->board[cell] == 0)
      d_ele->spaces[d_ele->nspaces++] = cell;
  }
}

void input(Datas* d_ele, const char in[N])
{
	d_ele->chess= (int (*)[COL])d_ele ->board;
  for (int cell = 0; cell < N; ++cell) {
    d_ele->board[cell] = in[cell]- '0';
    assert(0 <= d_ele->board[cell] && d_ele->board[cell] <= NUM);
  }
  find_spaces(d_ele);
}

bool available(Datas* d_ele, int guess, int cell)
{
  for (int i = 0; i < NEIGHBOR; ++i) {
    int neighbor =  d_ele->neighbors[cell][i];
    if ( d_ele->board[neighbor] == guess) {
      return false;
    }
  }
  return true;
}

bool solve_sudoku_basic(Datas* d_ele, int which_space)
{
  if (which_space >=  d_ele->nspaces) {
    return true;
  }

  // find_min_arity(which_space);
  int cell =  d_ele->spaces[which_space];

  for (int guess = 1; guess <= NUM; ++guess) {
    if (available(d_ele, guess, cell)) {
      // hold
      assert( d_ele->board[cell] == 0);
       d_ele->board[cell] = guess;

      // try
      if (solve_sudoku_basic(d_ele, which_space+1)) {
        return true;
      }

      // unhold
      assert( d_ele->board[cell] == guess);
       d_ele->board[cell] = 0;
    }
  }
  return false;
}
