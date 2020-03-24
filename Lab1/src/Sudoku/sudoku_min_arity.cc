#include <assert.h>

#include <algorithm>

#include "sudoku.h"

static int arity(Datas* d_ele, int cell)
{
  bool occupied[10] = {false};
  for (int i = 0; i < NEIGHBOR; ++i) {
    int neighbor = d_ele->neighbors[cell][i];
    occupied[d_ele->board[neighbor]] = true;
  }
  return std::count(occupied+1, occupied+10, false);
}

static void find_min_arity(Datas* d_ele, int space)
{
  int cell = d_ele->spaces[space];
  int min_space = space;
  int min_arity = arity(d_ele, cell);

  for (int sp = space+1; sp < d_ele->nspaces && min_arity > 1; ++sp) {
    int cur_arity = arity(d_ele,d_ele->spaces[sp]);
    if (cur_arity < min_arity) {
      min_arity = cur_arity;
      min_space = sp;
    }
  }

  if (space != min_space) {
    std::swap(d_ele->spaces[min_space], d_ele->spaces[space]);
  }
}

bool solve_sudoku_min_arity(Datas * d_ele, int which_space)
{
  if (which_space >= d_ele->nspaces) {
    return true;
  }

  find_min_arity(d_ele, which_space);
  int cell = d_ele->spaces[which_space];

  for (int guess = 1; guess <= NUM; ++guess) {
    if (available(d_ele, guess, cell)) {
      // hold
      assert(d_ele->board[cell] == 0);
      d_ele->board[cell] = guess;

      // try
      if (solve_sudoku_min_arity(d_ele, which_space+1)) {
        return true;
      }

      // unhold
      assert(d_ele->board[cell] == guess);
      d_ele->board[cell] = 0;
    }
  }
  return false;
}
