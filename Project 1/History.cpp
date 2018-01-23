
#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    //initialize unconverted array with 0
    for(int i = 1; i <= nRows; i++)
        for(int j = 1; j <= nCols; j++)
        {
            unconverted[i][j] = 0;
        }
}

bool History::record(int r, int c) {
    if(r > m_rows || c > m_cols)
        return false;
    //increments the number of unconverted flatulans at the position r, c
    //in the unconverted array
    int num = unconverted[r][c];
    unconverted[r][c] = num + 1;
    return true;
}

void History::display() const {
    // Position (row,col) in the city coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
            grid[r][c] = '.';
 
    // Indicate history of unconverted flatulans
    for (int i = 1; i <= m_rows; i++)
        for(int j = 1; j <= m_cols; j++)
    {
        char& gridChar = grid[i-1][j-1];
        int num = unconverted[i][j];
        if(num > 0 && num <= 26)
            gridChar = 64 + num;
        else if(num != 0 && num > 26)
            gridChar = 'Z';
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}

