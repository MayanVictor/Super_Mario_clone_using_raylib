#include "playerMario.hpp"
#include <raylib.h>
#include <raymath.h>

Mario::Mario() 
{
    health = 1;
    position = { 100.0f, 425.0f };
    velocity = { 0.0f, 0.0f };
    acceleration = 900.0f;
    Hitbox = { position.x, position.y, 50.0f, 50.0f };
    gravity = 1400.0f;
    fallGravityMultiplier = 1.5f;
    maxMoveSpeed = 200.0f;
    jumpSpeed = 520.0f;
    groundFriction = 1100.0f;
    maxFallSpeed = 700.0f;
    direction = NONE;
    inGround = true;
}
void Mario::update(float deltaTime)
{
    if(direction == LEFT)
    {
        velocity.x -= acceleration * deltaTime;
    }
    else if(direction == RIGHT)
    {
        velocity.x += acceleration * deltaTime;
    }
    else
    {
        if(velocity.x > 0) {
            velocity.x -= groundFriction * deltaTime;
            if(velocity.x < 0) {
                velocity.x = 0;
            }
        }
        if(velocity.x < 0) {
            velocity.x += groundFriction * deltaTime;
            if(velocity.x > 0) {
                velocity.x = 0;
            }
        }
    }

    velocity.x = Clamp(
        velocity.x,
        -maxMoveSpeed,
        maxMoveSpeed
    );

    float currentGravity = gravity;

    if(velocity.y > 0.0f)
    {
        currentGravity *= fallGravityMultiplier;
    }

    velocity.y += currentGravity * deltaTime;

    if(velocity.y > maxFallSpeed)
    {
        velocity.y = maxFallSpeed;
    }

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    Hitbox.x = position.x;
    Hitbox.y = position.y;
}
void Mario::jump()
{
    if(inGround) 
    {
        velocity.y = -jumpSpeed;
        inGround = false;
    }
}
void Mario::stopJump()
{
    if(velocity.y < -150.0f)
    {
        velocity.y = -150.0f;
    }
}
Direction Mario::setDirection(Direction dir)
{
    direction = dir;
    return direction;
}
Rectangle Mario::getHitbox()
{
    return Hitbox;
}
bool Mario::IsInGround(bool inground)
{
    inGround = inground;
    return inGround;
}
bool Mario::headCollision(Rectangle head, Rectangle Block)
{
    if(CheckCollisionRecs(head, Block)) 
    {
        return true;
    }
    return false;
}
bool Mario::RightCollision(Rectangle playerright, Rectangle Block)
{
    if(CheckCollisionRecs(playerright, Block)) 
    {
        return true;
    }
    return false;
}
bool Mario::LeftCollision(Rectangle playerLeft, Rectangle Block) {
    if(CheckCollisionRecs(playerLeft, Block)) {
        return true;
    }
    return false;
}
void Mario::landOn(float floorY)
{
    position.y = floorY - Hitbox.height;
    velocity.y = 0;
    inGround = true;

    Hitbox.y = position.y;
}
bool Mario::isFalling() const
{
    return velocity.y >= 0.0f;
}
void Mario::draw()
{
    DrawRectangleRec(Hitbox, RED);
}