#include <bits/stdc++.h>
#include <fstream>
#include "lip/boost/archive/text_oarchive.hpp"
#include "lip/boost/archive/text_iarchive.hpp"
#include "lip/boost/serialization/vector.hpp"
#include "lip/boost/serialization/map.hpp"

using namespace std;

int const MAZE_SIZE=9;

vector<vector<char>> maze {{' ', ' ', ' ', 'W', 'W', 'W', 'W', 'W', ' '},
                           {' ', 'W', 'W', 'W', ' ', ' ', ' ', 'W', ' '},
                           {' ', 'W', 'G', ' ', 'B', ' ', ' ', 'W', ' '},
                           {' ', 'W', 'W', 'W', ' ', 'B', 'G', 'W', ' '},
                           {' ', 'W', 'G', 'W', 'W', 'B', ' ', 'W', ' '},
                           {' ', 'W', ' ', 'W', ' ', 'G', ' ', 'W', 'W'},
                           {' ', 'W', 'B', 'B', 'G', 'B', 'B', 'G', 'W'},
                           {' ', 'W', ' ', ' ', ' ', 'G', ' ', ' ', 'W'},
                           {' ', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}};
vector<pair<int,int> > goalLocation;
bool invalidPoint(int x,int y);
vector<pair<int,int> > generateNextStates(int x,int y);
void save( const string& filePath,map <vector<vector<char>>, map <char, int>> & data);
map <vector<vector<char>>, map <char, int>> load(const string& filePath);
void findGoalLocation();

int main()
{
    map <vector<vector<char>>, map <char, int>> m,n;
    map <char, int> b;
    b['c']=1;
    m[maze]=b;
    save("test",m);
    n= load("test");
    cout<<(n==m)<<endl;
    return 0;
}

void save( const string& filePath,map <vector<vector<char>>, map <char, int>> & data){
    ofstream ofs(filePath);
    boost::archive::text_oarchive oa(ofs);

    oa<<data;
}
map <vector<vector<char>>, map <char, int>> load(const string& filePath){
    map <vector<vector<char>>, map <char, int>> data;
    ifstream ifs(filePath);
    boost::archive::text_iarchive ia(ifs);
    ia>>data;
    return data;

}
// find location (x,y) of each goal point
void findGoalLocation(){
    for (int i = 0; i < MAZE_SIZE; ++i)
        for (int j = 0; j <MAZE_SIZE ; ++j)
            if(maze[i][j]=='G')
                goalLocation.emplace_back(i,j);
}

// generate the valid next state
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

            if(invalidPoint(_x,_y)||maze[_x][_y]=='B' )
                continue;
        }
        validState.emplace_back(next_x,next_y);
    }

    return validState;
}

bool invalidPoint(int x,int y){
    return x>=MAZE_SIZE || y>=MAZE_SIZE||maze[x][y]=='W' || x<0 || y<0;
}

bool isEndState(int x,int y){
    for (auto& i:goalLocation)
        if (maze[i.first][i.second]!='X')
            return false;

    return true;
}