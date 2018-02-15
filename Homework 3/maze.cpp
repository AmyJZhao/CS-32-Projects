bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if(sr < 0 || sc < 0 || er < 0 || ec < 0)
        return false;
    //if the start location is equal to the ending location, return true
    if(sr == er && sc == ec)
        return true;
    //mark the start location as visited
    maze[sr][sc] = 'D';
    //if east is unvisited, call pathExists
    if(sr >= 0 && sc + 1 >= 0 && sr < nRows && sc + 1 < nCols && maze[sr][sc + 1] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    }
    //if south is unvisited, call pathExists
    if(sr + 1 >= 0 && sc >= 0 && sr + 1 < nRows && sc < nCols && maze[sr + 1][sc] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    }
    //if north is unvisited, call pathExists
    if(sr >= 0 && sc - 1 >= 0 && sr < nRows && sc - 1 < nCols && maze[sr][sc - 1] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    }
    //if west is unvisited, call pathExists
    if(sr - 1 >= 0 && sc >= 0 && sr - 1 < nRows && sc < nCols && maze[sr - 1][sc] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    }
    return false;
}
