#include <raylib.h>
#include <raymath.h>
#include "Enemy.hpp"

Enemy::Enemy(EnemyType Type, Vector2 Position)
{
    type = Type;
    position = Position;
    velocity = {-100.0f, 0.0f};
    alive = true;
    gravity = 1400.0f;
    maxFallSpeed = 700.0f;
    active = false;
    hitbox = {position.x, position.y, 50.0f, 50.0f};
}
void Enemy::update(float deltaTime)
{
    if(!alive || !active)
        return;

    velocity.y += gravity * deltaTime;

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
    float eyebrowWidth = 25.0f;
    float eyebrowHeight = 8.0f;
    Rectangle leftEyebrow = {hitbox.x + 14, hitbox.y + 10, eyebrowWidth, eyebrowHeight};

    Rectangle rightEyebrow = {hitbox.x + 34, hitbox.y + 10, eyebrowWidth, eyebrowHeight};
    if(!alive) {
        return;
    }
    switch(type) {
        case EnemyType::GOOMBA: {
            DrawRectangleRec({hitbox.x, hitbox.y, hitbox.width, 30}, BROWN);
            DrawRectangleRec({hitbox.x + 10, hitbox.y + 30, hitbox.width - 10, 15}, BEIGE);
            DrawRectangleRec({hitbox.x - 10, hitbox.y + 35, 20, 15}, BLACK);
            DrawRectangleRec({hitbox.x + hitbox.width - 15, hitbox.y + 32, 20, 15}, BLACK);
            DrawRectangleRec({hitbox.x + 5, hitbox.y + 10, 15, 15}, RAYWHITE);
            DrawRectangleRec({hitbox.x + 30, hitbox.y + 10, 15, 15}, RAYWHITE);
            DrawRectangleRec({hitbox.x + 10, hitbox.y + 12, 10, 10}, BLACK);
            DrawRectangleRec({hitbox.x + 30, hitbox.y + 12, 10, 10}, BLACK);
            DrawRectanglePro(leftEyebrow, {leftEyebrow.width / 2.0f, leftEyebrow.height / 2.0f}, 15, BLACK);
            DrawRectanglePro(rightEyebrow, {rightEyebrow.width / 2.0f, rightEyebrow.height / 2.0f}, -15, BLACK);
            break;
        }
        case EnemyType::KOOPA: {
            DrawRectangleRec(hitbox, GREEN);
            DrawRectangleRec({hitbox.x - 10, hitbox.y - 20, 30, 50}, BEIGE);
            break;
        }
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
void Enemy::setActive(bool Active)
{
    active = Active;
}
bool Enemy::isActive() const
{
    return active;
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