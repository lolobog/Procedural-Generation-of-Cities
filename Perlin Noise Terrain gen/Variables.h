#pragma once
#include <math.h>

const int ScreenWidth = 1500;
const int ScreenHeight = 1000;
const int imgWidth = 1000, const imgHeight = 1000;
const int chunkWidth = 200, const chunkHeight = 200;

float distanceF(int x1, int y1, int x2, int y2);

class Singleton
{
private:
    Singleton() {};

public:

    static Singleton* Instance()
    {
        static Singleton* instance;

        if (instance == NULL)
            instance = new Singleton();

        return instance;
    }


};


