#include <raylib.h>
#include <raymath.h>
#include "Enemy.hpp"

Enemy::Enemy(EnemyType type, Vector2 position)
    : type(type), position(position), velocity({-100.0f, 0.0f}), alive(true), gravity(1400.0f), maxFallSpeed(700.0f)
{
    hitbox = {position.x, position.y, 50.0f, 50.0f};
}
void Enemy::update(float deltaTime)
{
    if(!alive)
        return;


    float currentGravity = gravity;

    velocity.y += currentGravity * deltaTime;

    if(velocity.y > maxFallSpeed)
    {
        velocity.y = maxFallSpeed;
    }
    // Update enemy position based on velocity
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    // Update hitbox position
    hitbox.x = position.x;
    hitbox.y = position.y;
}
void Enemy::draw()
{
    if(!alive)
        return;

    switch(type) {
        case EnemyType::GOOMBA:
            DrawRectangleV(position, {50, 50}, BROWN);
            break;
        case EnemyType::KOOPA:
            DrawRectangleV(position, {50, 50}, GREEN);
            break;
        case EnemyType::PIRANHA_PLANT:
            DrawRectangleV(position, {50, 50}, RED);
            break;
        default:
            break;
    }
}
void Enemy::LandOn(float floorY)
{
    position.y = floorY - hitbox.height;
    velocity.y = 0;

    hitbox.y = position.y;
}
void Enemy::hitWallOnRight(float blockLeft)
{
    position.x = blockLeft - hitbox.width;
    velocity.x = -fabs(velocity.x);
    hitbox.x = position.x;
}
void Enemy::hitWallOnLeft(float blockRight)
{
    position.x = blockRight;
    velocity.x = fabs(velocity.x);
    hitbox.x = position.x;
}
void Enemy::invertDirection()
{
    velocity.x *= -1.0f;
}
void Enemy::setX(float x)
{
    position.x = x;
    hitbox.x = x;
}
float Enemy::getVelocityX() const
{
    return velocity.x;
}
Rectangle Enemy::getGroundCheck() const
{
    return {hitbox.x, hitbox.y + hitbox.height, hitbox.width, 3};
}
Rectangle Enemy::getLeftCheck() const
{
    return {hitbox.x - 3, hitbox.y + 5, 3, hitbox.height - 10};
}
Rectangle Enemy::getRightCheck() const
{
    return {hitbox.x + hitbox.width, hitbox.y + 5, 3, hitbox.height - 10};
}
Rectangle Enemy::getHitbox() const
{
    return hitbox;
}
EnemyType Enemy::getType() const
{
    return type;
}
bool Enemy::isAlive() const
{
    return alive;
}

void Enemy::defeat()
{
    alive = false;
}