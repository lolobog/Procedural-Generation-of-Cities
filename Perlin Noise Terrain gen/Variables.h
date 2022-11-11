#pragma once

const int ScreenWidth = 1000;
const int ScreenHeight = 1000;
const int imgWidth = 1000, const imgHeight = 1000;
const int chunkWidth = 200, const chunkHeight = 200;


class Singleton
{
private:
    

public:
    Singleton() {};

    static Singleton* Instance()
    {
        static Singleton* instance;

        if (instance == NULL)
            instance = new Singleton();

        return instance;
    }


};
