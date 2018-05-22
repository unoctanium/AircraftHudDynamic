// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GameOfLife.h"
#include "AircraftHudDrawingCanvas.h"



#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


GameOfLife::GameOfLife()
{
    srand (time(NULL));
    for (int u=0; u< WIDTH; u++)
        for (int v=0; v<HEIGHT; v++)
        {
            world[u][v]= '.';
            otherWorld[u][v]= (rand() % 3) > 1 ? 'X' : '.' ;
        }
}

void GameOfLife::Update()
{
    if(toggle)
    {
        for (int u=0; u< WIDTH; u++)
            for (int v=0; v<HEIGHT; v++)
                otherWorld[u][v] = GetState(world[u][v], u, v, toggle);
        toggle = !toggle;
    }
    else{
        for (int u=0; u< WIDTH; u++)
            for (int v=0; v<HEIGHT; v++)
                world[u][v] = GetState(otherWorld[u][v], u, v, toggle);
        toggle = !toggle;
    }
}

char GameOfLife::GetState(char state, int x, int y, bool _toggle)
{
    int neighbors = 0;
    if (_toggle)
    {
        for (int u = x-1; u <= x+1; u++)
            for (int v = y-1; v <= y+1; v++)
            {
                if(u == x && v == y) {
                    continue;
                }
                if (u > -1 && u < WIDTH && v > -1 && v < HEIGHT)
                {
                    if (world[u][v] == 'X') {
                        neighbors ++;
                    }
                }
            }
    }
    else
    {
        for (int u = x-1; u <= x+1; u++)
            for (int v = y-1; v <= y+1; v++)
            {
                if(u == x && v == y) {
                    continue;
                }
                if (u > -1 && u < WIDTH && v > -1 && v < HEIGHT)
                {
                    if (otherWorld[u][v] == 'X') {
                        neighbors ++;
                    }
                }
            }
    }
    if (state == 'X')
    {
        return (neighbors > 1 && neighbors < 4) ? 'X' : '.';
    }
    else
    {
        return (neighbors == 3) ? 'X' : '.';
    }

}

bool GameOfLife::GetCell(int x, int y)
{
    if (toggle)
    {
        return world[x][y] == 'X';
    }
    else
    {
        return otherWorld[x][y] == 'X';
    }
}



