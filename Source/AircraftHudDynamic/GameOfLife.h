// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#define WIDTH 256
#define HEIGHT 256

class GameOfLife {

public: 

    GameOfLife();
    void Update();
    bool GetCell(int x, int y);
    char GetState(char state, int x, int y, bool _toggle);

private:

    char world[WIDTH][HEIGHT];
    char otherWorld[WIDTH][HEIGHT];
    bool toggle;
};

