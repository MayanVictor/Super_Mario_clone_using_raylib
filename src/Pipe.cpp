#include <raylib.h>
#include "Pipe.hpp"

Pipe::Pipe(Rectangle rect, PipeType Type, Vector2 dest, PipeDestiny gamearea, PipeDirection dir)
{
    type = Type;
    hitbox = rect;
    arrival = dest;
    destiny = gamearea;
    direction = dir;
}
void Pipe::draw()
{
    Rectangle body;
    Rectangle mouth;
    if(direction == PipeDirection::UP) {
        body = {hitbox.x + 5, hitbox.y + 15, hitbox.width - 10, hitbox.height - 15};
        mouth = {hitbox.x, hitbox.y, hitbox.width, 20};
    }
    else {
        body = {hitbox.x + 15, hitbox.y + 5, hitbox.height - 10, hitbox.width - 15 };
        mouth = {hitbox.x, hitbox.y, 20, hitbox.height};
    }
    DrawRectangleRec(body, GREEN);
    DrawRectangleRec(mouth, DARKGREEN);
}
Rectangle Pipe::getEntranceCheck()
{
    switch(direction)
    {
        case PipeDirection::UP:
            return {
                hitbox.x + 3,
                hitbox.y - 3,
                hitbox.width - 6,
                5
            };

        case PipeDirection::LEFT:
            return {
                hitbox.x - 3,
                hitbox.y + 3,
                5,
                hitbox.height - 6
            };

    }
    

    return hitbox;
}
Rectangle Pipe::getRightCheck()
{
    return {hitbox.x + hitbox.width, hitbox.y + 3, 6, hitbox.height};
}
Rectangle Pipe::getLeftCheck()
{
    return {hitbox.x - 3, hitbox.y + 3, 6, hitbox.height};
}
Rectangle Pipe::getHitbox() 
{
    return hitbox;
} 
PipeType Pipe::getType() const
{
    return type;
}
Vector2 Pipe::getDestination()
{
    return arrival;
}
PipeDestiny Pipe::getGameArea() const
{
    return destiny;
}
PipeDirection Pipe::getDirection() const
{
    return direction;
}