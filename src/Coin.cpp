#include "Coin.hpp"
#include <raylib.h>

Coin::Coin(Vector2 pos)
{
    position = pos;
    hitbox = {position.x, position.y, 40, 40};
    exist = true;
}
void Coin::draw()
{
    if(!exist) return;
    DrawCircle(hitbox.x + (hitbox.width / 2), hitbox.y + (hitbox.height / 2), 20, YELLOW);
}
void Coin::collectCoin()
{
    exist = false;
}
bool Coin::isExisting() const
{
    return exist;
}
Rectangle Coin::getHitbox() 
{
    return hitbox;
}