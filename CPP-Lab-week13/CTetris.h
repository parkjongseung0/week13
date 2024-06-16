#pragma once
#include <iostream>
#include <cstdlib>
#include "Tetris.h"

using namespace std;

class CTetris : public Tetris {
private:
    // static members
    static Matrix ***setOfColorBlockObjects;

    // dynamic members
    Matrix *iCScreen;
    Matrix *oCScreen; 
    Matrix *currCBlk;

public:
    static void init(int **setOfColorBlockArrays, int nTypes, int nDegrees);
    static void deinit(void);
    CTetris(int cy, int cx);
    ~CTetris();

    // accessors
    Matrix *get_oCScreen(void) const {  return oCScreen; }
    Matrix *get_iCSCreen(void) const { return iCScreen;}

    // mutators
    TetrisState accept(char key, Matrix *in, Matrix **out);
};