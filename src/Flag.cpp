#include "Flag.hpp"
#include <raylib.h>

Flag::Flag()
{
    position = {0, 0};
    flagPole = {position.x, position.y, 5, 300};
    flag = {position.x - 20, position.y, 20, 20};
    isCaptured = false;
}
void Flag::draw()
{
    DrawRectangleRec(flagPole, GREEN);
    DrawTriangle({flag.x, flag.y + flag.height / 2.0f}, {flag.x + flag.width, flag.y} ,{position.x + flag.width, flag.y + flag.height}, RAYWHITE);
    DrawCircle(position.x + 5, position.y - 10, 10, LIME);
}
void Flag::loweringTheFlag(float deltaTime)
{
    if(isCaptured)
    {
        if(flag.y < flagPole.y + flagPole.height - 20)
        {
            flag.y += 200.0f * deltaTime;
        }
    }
}
void Flag::setPosition(Vector2 pos)
{
    position.x = pos.x + 20;
    position.y = pos.y;
    flagPole = {position.x, position.y, 10, 350};
    flag = {position.x - 20, position.y, 20, 20};
}
Rectangle Flag::getFlagPoleHitbox() const
{
    return flagPole;
}