#pragma once
#include <raylib.h>

class Coin
{
    public:
        Coin(Vector2 pos);
        Rectangle getHitbox();
        void draw();
        bool isExisting() const;
        void collectCoin();
    private:
        Rectangle hitbox;
        Vector2 position;

        bool exist;
};