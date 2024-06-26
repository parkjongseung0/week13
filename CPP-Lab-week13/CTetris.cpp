﻿#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** CTetris::setOfColorBlockObjects = NULL;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function

  setOfColorBlockObjects = new Matrix**[numTypes]; // allocate 1d array of pointers to Matrix pointer arrays

  for (int t = 0; t < numTypes; t++)
    setOfColorBlockObjects[t] = new Matrix*[numDegrees]; // allocate 1d array of Matrix pointers

  for (int t = 0; t < numTypes; t++) {
    int size = setOfBlockObjects[t][0]->get_dy();
    for (int d = 0; d < numDegrees; d++) { // allocate matrix objects
      setOfColorBlockObjects[t][d] = new Matrix(setOfColorBlockArrays[numDegrees * t + d], size, size);
      //cout << *setOfBlockObjects[t][d] << endl;
    }
  }
}

void CTetris::deinit(void) {
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL;
}

CTetris::~CTetris() {
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key, Matrix *in, Matrix **out) {
  Matrix *tempBlk, *tempBlk2;

  TetrisState _state = Tetris::accept(key,in,out); // call superclass' function

  currCBlk = setOfColorBlockObjects[type][degree];

  // perform addition between currCBlk and iCScreen
  tempBlk = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
  tempBlk2 = tempBlk->add(currCBlk);
  delete tempBlk;

  // update oCScreen
  Matrix * del = iCScreen->clip(0,0,oCScreen->get_dy(),oCScreen->get_dx());
  oCScreen->paste(iCScreen, 0, 0);
  oCScreen->paste(tempBlk2, top, left);
  delete tempBlk2;
  Matrix **pout = out;
  
  if (_state == TetrisState::NewBlock) {
    oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth,in,del);
    if(in->get_dx()==oCScreen->get_dx() - wallDepth*2){
      Matrix *temp = (*pout)->clip(in->get_dy(), wallDepth, oCScreen->get_dy()-wallDepth, oCScreen->get_dx() - wallDepth);
      (*pout)->paste(temp, 0, wallDepth);
      (*pout)->paste(in, oCScreen->get_dy()-wallDepth-in->get_dy(),wallDepth);
    }
    iCScreen->paste(oCScreen, 0, 0);
  }

  Matrix *bscreen = iCScreen->int2bool();
  iScreen->paste(bscreen,0,0);
  delete bscreen;

  return _state;
}