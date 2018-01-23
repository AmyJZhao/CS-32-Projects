
#ifndef HISTORY_H
#define HISTORY_H
#include "globals.h"

//History's interface

class History {
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_rows;
        int m_cols;
        //array of display grid that will indicate where the unconverted flatulans are
        int unconverted[MAXROWS][MAXCOLS];
};

#endif

