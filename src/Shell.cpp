#include "Shell.hpp"
#include <raylib.h>

Shell::Shell(Vector2 inicialPosition)
{
    position = inicialPosition;
    state = ShellState::IDLE;
    hitbox = {position.x, position.y +20, 50, 30};
    direction = ShellDirection::NONE;
    velocity = {0.0f, 0.0f};
    moving = false;
    exist = true;
    gravity = 1400.0f;
    kickcooldown = 0.0f;
}
void Shell::update(float deltatime)
{
    if(!exist) return;

    if(kickcooldown > 0.0f)
    {
        kickcooldown -= deltatime;
    }

    if(moving)
    {
        position.x += velocity.x * deltatime;
    }
    velocity.y += gravity * deltatime;
    if(velocity.y > 700.0f)
    {
        velocity.y = 700.0f;
    }

    position.y += velocity.y * deltatime;

    hitbox.x = position.x;
    hitbox.y = position.y;
}
void Shell::Kick(ShellDirection dir)
{
    direction = dir;
    state = ShellState::MOVING;
    moving = true;
    kickcooldown = 0.15f;

    if(direction == ShellDirection::RIGHT)
    {
        velocity.x = 400.0f;
    }
    if(direction == ShellDirection::LEFT) 
    {
        velocity.x = -400.0f;
    }
    
}
void Shell::LandOn(float floorY)
{
    position.y = floorY - hitbox.height;
    velocity.y = 0;

    hitbox.y = position.y;
}
void Shell::Stop()
{
    velocity.x = 0.0f;
    state = ShellState::IDLE;
    moving = false;
}
void Shell::draw()
{
    if(!exist) return;

    DrawRectangleRec(hitbox, DARKGREEN);
}
void Shell::invertDirection()
{
    velocity.x *= -1.0f;
}
void Shell::destroy()
{
    exist = false;
}
bool Shell::isMoving() const
{
    return moving;
}
bool Shell::isExisting() const
{
    return exist;
}
bool Shell::isFalling() const
{
    return velocity.y > 0.0f;
}
bool Shell::canDamageMario() const
{
    return kickcooldown <= 0.0f;
}
ShellState Shell::getShellState() const
{
    return state;
}
Rectangle Shell::getHitBox()
{
    return hitbox;
}
Rectangle Shell::getGroundCheck()
{
    return {hitbox.x + 3, hitbox.y + hitbox.height, hitbox.width - 6, 3};
}
Rectangle Shell::getTopCheck()
{
    return {hitbox.x + 3, hitbox.y - 3, hitbox.width - 6, 3};
}
Rectangle Shell::getRightCheck()
{
    return {hitbox.x + hitbox.width, hitbox.y + 3, 3, hitbox.height - 6};
}
Rectangle Shell::getLeftCheck()
{ 
    return {hitbox.x - 3, hitbox.y + 3, 3, hitbox.height - 6};
}