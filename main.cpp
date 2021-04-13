#include <bits/stdc++.h>

using namespace std;

int const MAZE_SIZE=9;

vector<vector<int> > maze {{' ',' ',' ','W','W','W','W','W',' '},
                           {' ','W','W','W',' ',' ',' ','W',' '},
                           {' ','W','G',' ','B',' ',' ','W',' '},
                           {' ','W','W','W',' ','B','G','W',' '},
                           {' ','W','G','W','W','B',' ','W',' '},
                           {' ','W',' ','W',' ','G',' ','W','W'},
                           {' ','W','B','B','G','B','B','G','W'},
                           {' ','W',' ',' ',' ','G',' ',' ','W'},
                           {' ','W','W','W','W','W','W','W','W'}};
vector<pair<int,int> > goalLocation;
bool invalidPoint(int x,int y);
vector<pair<int,int> > generateNextStates(int x,int y);
void findGoalLocation();

int main()
{
    findGoalLocation();

    return 0;
}

// find location (x,y) of each goal point
void findGoalLocation(){
    for (int i = 0; i < MAZE_SIZE; ++i)
        for (int j = 0; j <MAZE_SIZE ; ++j)
            if(maze[i][j]=='G')
                goalLocation.emplace_back(i,j);
}

vector<pair<int,int> > generateNextStates(int x,int y){
    vector<pair<int,int> > validState;
    int x_move[] ={0,1,-1,0};
    int y_move[] ={1,0,0,-1};

    for (int i = 0; i < 4; ++i) {
        int next_x = x+x_move[i];
        int next_y = y+y_move[i];

        if(invalidPoint(next_x,next_y))
            continue;
        if (maze[next_x][next_y]=='B'){
            int _x = x+x_move[i];
            int _y = y+y_move[i];

            if(invalidPoint(_x,_y)||maze[_x][_y]=='B')
                continue;
        }
        validState.emplace_back(next_x,next_y);
    }

    return validState;
}

bool invalidPoint(int x,int y){
    return x>=MAZE_SIZE || y>=MAZE_SIZE||maze[x][y]=='W';
}

bool isEndState(int x,int y){
    for (auto& i:goalLocation)
        if (maze[i.first][i.second]!='X')
            return false;

    return true;
}