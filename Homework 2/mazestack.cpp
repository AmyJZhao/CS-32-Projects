#include <stack>
#include <iostream>
#include <string>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    Coord startCoord(sr, sc);
    coordStack.push(startCoord);
    maze[sr][sc] = 'D';
    while(!coordStack.empty())
    {
        Coord temp = coordStack.top();
        coordStack.pop();
        if(temp.r() == er && temp.c() == ec)
        {
            return true;
        }
        if(temp.r() < nRows && temp.c() + 1 < nCols && maze[temp.r()][temp.c() + 1] == '.')
        {
            coordStack.push(Coord(temp.r(), temp.c() + 1));
            maze[temp.r()][temp.c() + 1] = 'D';
        }
        if(temp.r() + 1 < nRows && temp.c() < nCols && maze[temp.r() + 1][temp.c()] == '.')
        {
            coordStack.push(Coord(temp.r() + 1, temp.c()));
            maze[temp.r() + 1][temp.c()] = 'D';
        }
        if(temp.r() < nRows && temp.c() - 1 < nCols && temp.c() - 1 >= 0 && maze[temp.r()][temp.c() - 1] == '.')
        {
            coordStack.push(Coord(temp.r(), temp.c() - 1));
            maze[temp.r()][temp.c() - 1] = 'D';
        }
        if(temp.r() - 1 >= 0 && temp.r() - 1 < nRows && temp.c() <= nCols - 1 && maze[temp.r() - 1][temp.c()] == '.')
        {
            coordStack.push(Coord(temp.r() - 1, temp.c()));
            maze[temp.r() - 1][temp.c()] = 'D';
        }
    }
    return false;
}
