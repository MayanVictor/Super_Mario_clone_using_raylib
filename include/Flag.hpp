#pragma once
#include <raylib.h>

class Flag
{
    public:
        Flag();
        void draw();
        void loweringTheFlag(float deltaTime);
        void setPosition(Vector2 pos);
        Rectangle getFlagPoleHitbox() const;
    private:
        Vector2 position;
        Rectangle flagPole;
        Rectangle flag;
        bool isCaptured;
};