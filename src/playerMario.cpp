#include "playerMario.hpp"
#include <raylib.h>
#include <raymath.h>

Mario::Mario() 
{
    lifes = 3;
    coins = 0;
    position = { 100.0f, 425.0f };
    velocity = { 0.0f, 0.0f };
    acceleration = 900.0f;
    Hitbox = { position.x, position.y, 50.0f, 50.0f };
    gravity = 1400.0f;
    fallGravityMultiplier = 1.5f;
    maxMoveSpeed = 200.0f;
    jumpSpeed = 750.0f;
    groundFriction = 1100.0f;
    maxFallSpeed = 700.0f;
    direction = NONE;
    inGround = true;
    isGrowth = false;
    state = MarioState::SMALL;
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
    if(velocity.y < -400.0f)
    {
        velocity.y = -400.0f;
    }
}
void Mario::hitCeiling(float blockBottom)
{
    position.y = blockBottom;
    velocity.y = 0.0f;

    Hitbox.y = position.y;
}
void Mario::loselife()
{
    if(lifes > 0)
    {
        lifes--;
    }
}
void Mario::gainlife()
{
    lifes++;
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
void Mario::landOn(float floorY)
{
    position.y = floorY - Hitbox.height;
    velocity.y = 0;
    inGround = true;

    Hitbox.y = position.y;
}
void Mario::MarioGrowth()
{
    if(isGrowth) {
        return; // Already in BIG state, do nothing
    }
    state = MarioState::BIG;
    Hitbox.height = 100;
    isGrowth = true;
}
void Mario::MarioFire()
{
    state = MarioState::FIRE;
    Hitbox.height = 100;
}
void Mario::hitWallOnRight(float blockLeft)
{
    position.x = blockLeft - Hitbox.width;
    velocity.x = 0.0f;
    Hitbox.x = position.x;
}

void Mario::hitWallOnLeft(float blockRight)
{
    position.x = blockRight;
    velocity.x = 0.0f;
    Hitbox.x = position.x;
}
int Mario::getLifes() const
{
    return lifes;
}
int Mario::getCoins() const
{
    return coins;
}
void Mario::resetCoins()
{
    coins = 0;
}
void Mario::addCoin()
{
    coins++;
}
bool Mario::isFalling() const
{
    return velocity.y >= 0.0f;
}
MarioState Mario::getState() const
{
    return state;
}
void Mario::draw()
{
    if(state == MarioState::SMALL) {
        DrawRectangleRec(Hitbox, RED);
    } else if(state == MarioState::BIG) {
        DrawRectangleRec(Hitbox, GREEN);
    } else if(state == MarioState::FIRE) {
        DrawRectangleRec(Hitbox, ORANGE);
    }
}