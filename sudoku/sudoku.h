#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <map>
using namespace std;

class sudoku
{
private:

    int size;
    int level;
    int last_hint=-1;
    bool was_hint=false;
    bool no_solution=false;

protected:

    vector <int> tab;
    vector <int> solved;
    vector <bool> fixed;
    vector <vector <bool>> vis;
    vector <int> filled;
    vector <bool> prefilled;

public:

    sudoku() {};

    int getCell(int a);
    int getSize();
    int getLevel();
    int getLast_hint();
    bool emptyCell(int &a);
    bool possibleDigit(int a, int n);
    bool done();
    bool dfs(int a);
    void solve();
    bool good();
    bool win();
    void hint();
    void setSize(int size);
    void setLevel(int level);
    void reset_hint();
    bool wasHint();
    void setLast_hint(int last_hint);
    bool noSolution();
    void setNoSolution(bool a);
    void importCell(int index, int n, int size);
    bool wasPrefilled(int index);
    void insert(int w, int k, int n);
    void remove();
    virtual void getFile(int level)=0;
    virtual void show()=0;
};

class sudoku4x4 :public sudoku
{
public:
    sudoku4x4() :sudoku() {};
    void show();
    void getFile(int level);
};

class sudoku9x9 :public sudoku
{
public:
    sudoku9x9() :sudoku() {};
    void show();
    void getFile(int level);
};

class sudoku16x16 :public sudoku
{
public:
    sudoku16x16() :sudoku() {};
    void show();
    void getFile(int level);
};

class player
{
private:
    string name="";
    double time;
    int hint_count=0;
public:
    string getName();
    double getTime();
    int getHint_count();
    void setName(string name);
    void stopTime();
    void addHint();

};

class game
{
private:
    map <pair<int,int>,vector<pair<string,double>>> board;
public:
    void main_menu();
    void new_game();
    void choose_level(int size);
    player *newPlayer();
    sudoku *start(int size, int level);
    sudoku *import(int size);
    void play(sudoku *ptr, player *p1);
    void leaderboard1();
    void leaderboard2(int size);
    void addToBoard(int size, int level, string name, double time);
    void showBoard(int size, int level);
    void help();
    void rules(sudoku *ptr, player *p1);
};