#include "FireBall.hpp"
#include <raylib.h>

FireBall::FireBall(Vector2 startposition, ShootDirection direction)
{
    position.x = startposition.x;
    position.y = startposition.y + 40.0f;
    hitbox = {position.x, position.y, 20.0f, 20.0f};
    if(direction == ShootDirection::RIGHT)
    {
        dirMultiplicator = 1.0f;
    } else {
        dirMultiplicator = -1.0f;
    }
    velocity = {300.0f * dirMultiplicator, 150.0f};
    gravity = 900.0f;
    bounce = 100.0f;
    active = true;
}
void FireBall::draw()
{
    if(!active) return;

    DrawRectangleRec(hitbox, ORANGE);
}
void FireBall::update(float deltatime)
{
    if(!active) return;

    velocity.y += gravity * deltatime;

    if(velocity.y > 350.0f)
    {
        velocity.y = 350.0f;
    }

    position.x += velocity.x * deltatime;
    position.y += velocity.y * deltatime;


    hitbox.x = position.x;
    hitbox.y = position.y;
}
void FireBall::Bounce()
{
    velocity.y = -250.0f;
}
Rectangle FireBall::getGroundCheck()
{
    return {hitbox.x, hitbox.y + hitbox.height, hitbox.width, 3};
}
Rectangle FireBall::getFrontCheck()
{
    return {hitbox.x + hitbox.width, hitbox.y, 3, hitbox.height - 3};
}
Rectangle FireBall::getBackCheck()
{
    return {hitbox.x - 3, hitbox.y, 3, hitbox.height - 3};
}
Rectangle FireBall::getHitbox()
{
    return hitbox;
}
bool FireBall::isActive()
{
    return active;
}
void FireBall::destroy()
{
    active = false;
}