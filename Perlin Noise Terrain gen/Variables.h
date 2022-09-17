#pragma once

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

    int ScreenWidth=1000;
    int ScreenHeight=1000;
};
