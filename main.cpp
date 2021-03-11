#include <iostream>
#include <pthread.h>
#include <vector>
#include <array>
#include <typeinfo>
#include <bits/stdc++.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;
int gridSz = 5;
char grid[5][5];
//bool stop;
string endMsg;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void printWorld()
{
    pthread_mutex_lock(&mutex);
    cout << "\n";
    bool deadWalle = true;
    bool deadGold = true;
    for(int i = 0; i < gridSz; i++)
    {
        for(int j = 0; j < gridSz; j++)
        {
            if(grid[i][j] == 'W')
            {
                deadWalle = false;
            }
            else if(grid[i][j] == 'G')
            {
                deadGold = false;
            }
            cout << grid[i][j] << "  ";
        }
        cout << endl;
    }
    if(deadWalle)
    {
        /*endMsg = "You lose walle";
        stop = true;*/
        cout << "\nKABOOM! Bomb 'sploded the Walle.\n";
        exit(0);

    }
    else if(deadGold)
    {
        /*endMsg = "you win walle";
        stop = true;*/
        cout << "\nCongrats. Walle got his gold.\n";
        exit(0);
    }
    pthread_mutex_unlock(&mutex);
}

class Piece
{
    private:

        void addToPos(int px, int py)
        {
            pthread_mutex_lock(&mutex);
            //This makes the object avoid the thing being avoided by holding still if it is set to move in that direction
            if(grid[x + px][y + py] == avoid)
            {
                px  = 0;
                py = 0;
            }
            grid[x + px][y + py] = obj;
            if(px != 0 || py != 0)
            {
                grid[x][y] = '_';
            }
            x += px;
            y += py;
            pthread_mutex_unlock(&mutex);
        }

    public:
        int x;
        int y;
        int bb;
        int tb;
        int lb;
        int rb;
        char goal;
        char avoid;
        char obj;
        int timeToMove;

        Piece(){}
        ~Piece(){}

        void initPiece(int px, int py, char name, int bottom, int top, int left, int right)
        {
            obj = name;
            grid[px][py] = obj;
            x = px;
            y = py;
            bb = bottom;
            tb = top;
            lb = left;
            rb = right;
        }

        void moveOver(int px, int py)
        {
            if(!(y == lb && py < 0
               ||y == rb && py > 0
               ||x == tb && px < 0
               ||x == bb && px > 0))
            {
                addToPos(px, py);
            }
        }
};

class World
{
    public:
        World(){}
        ~World(){}

        void create()
        {
            for(int i = 0; i < gridSz; i++)
            {
                for(int j = 0; j < gridSz; j++)
                {
                    grid[i][j] = '_';
                }
            }

        }
};

int getRand(int nums, int mid)
{
    int rnd = rand() % nums - mid;
    return rnd;
}

void* walleThread (void* arg)
{
    Piece piece = *(Piece*) arg;
    while(true)
    {
        Sleep(piece.timeToMove);
        piece.moveOver(getRand(3,1),getRand(3,1));
        printWorld();
    }
    pthread_exit(NULL);
}

int main()
{
    srand(time(0));
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    World earth;
    earth.create();

    Piece w;
    Piece b;
    Piece g;

    int gx = gridSz/2;
    int gy = gridSz/2;
    int bx = gridSz/2;
    int by = gridSz/2;
    while(gx == bx && gy == by)
    {
        gx = getRand(gridSz,0);
        gy = getRand(gridSz,0);
        bx = getRand(gridSz,0);
        by = getRand(gridSz,0);
    }

    g.initPiece(gx, gy, 'G', gridSz - 1, 0, 0, gridSz - 1);
    w.initPiece(gridSz/2, gridSz/2, 'W', gridSz - 1, 0, 0, gridSz - 1);
    b.initPiece(bx, by, 'B', gridSz - 1, 0, 0, gridSz - 1);
    w.goal = g.obj;
    b.goal = w.obj;
    g.goal = ' ';
    w.avoid = b.obj;
    b.avoid = g.obj;
    w.timeToMove = 1000;
    b.timeToMove = w.timeToMove * 2;

    printWorld();
    pthread_t walle;
    pthread_t bomb;

    pthread_create(&walle, NULL, walleThread, &w);
    pthread_create(&bomb, NULL, walleThread, &b);

    pthread_join(walle, NULL);
    pthread_join(bomb,NULL);
    return -1;
}
