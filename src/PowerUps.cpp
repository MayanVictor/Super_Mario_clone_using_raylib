#include "PowerUps.hpp"
#include <raylib.h>
#include <raymath.h>

PowerUp::PowerUp(PowerUpType type, Vector2 position)
    : type(type),
      position(position),
      velocity({0.0f, 0.0f}),
      state(PowerUpState::INACTIVE)
{
    hitbox = {position.x, position.y, 32.0f, 32.0f};

    targetY = position.y;
    gravity = 1400.0f;
    emergeSpeed = 100.0f;
}
void PowerUp::update(float deltaTime)
{
    if(state == PowerUpState::INACTIVE)
    {
        return;
    }

    if(state == PowerUpState::EMMERGING)
    {
        position.y -= emergeSpeed * deltaTime;

        if(position.y <= targetY)
        {
            position.y = targetY;
            state = PowerUpState::ACTIVE;

            if(type == PowerUpType::MUSHROOM)
            {
                velocity.x = 100.0f;
            }
            else if(type == PowerUpType::STAR)
            {
                velocity.x = 150.0f;
            }
        }
    }
    else if(state == PowerUpState::ACTIVE)
    {
        if(type == PowerUpType::MUSHROOM)
        {
            velocity.y += gravity * deltaTime;

            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
        else if(type == PowerUpType::FIRE_FLOWER)
        {
            // fica parado
        }
        else if(type == PowerUpType::STAR)
        {
            velocity.y += gravity * deltaTime;

            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
        }
    }

    hitbox.x = position.x;
    hitbox.y = position.y;
}
void PowerUp::draw()
{
    if(state == PowerUpState::INACTIVE) {
        return;
    }
    switch(type) {
        case PowerUpType::MUSHROOM:
            DrawRectangleV(position, {32, 32}, RED);
            break;
        case PowerUpType::FIRE_FLOWER:
            DrawRectangleV(position, {32, 32}, ORANGE);
            break;
        case PowerUpType::STAR:
            DrawRectangleV(position, {32, 32}, YELLOW);
            break;
        default:
            break;
    }
}
void PowerUp::landOn(float floorY)
{
    position.y = floorY - hitbox.height;

    if(type == PowerUpType::STAR)
    {
        velocity.y = -450.0f;
    }
    else
    {
        velocity.y = 0.0f;
    }

    hitbox.y = position.y;
}
void PowerUp::generate(Vector2 blockPosition)
{
    position.x = blockPosition.x + 8.0f; // Center the power-up on the block
    position.y = blockPosition.y;

    hitbox.x = position.x;
    hitbox.y = position.y;

    targetY = position.y - hitbox.height; // Move up by 32 units
    
    state = PowerUpState::EMMERGING;
}
void PowerUp::hitWallOnRight(float blockLeft)
{
    position.x = blockLeft - hitbox.width;
    velocity.x = -fabs(velocity.x);
    hitbox.x = position.x;
}
void PowerUp::hitWallOnLeft(float blockRight)
{
    position.x = blockRight;
    velocity.x = fabs(velocity.x);
    hitbox.x = position.x;
}
void PowerUp::collect()
{
    state = PowerUpState::INACTIVE;
}
bool PowerUp::isActive() const
{
    return state == PowerUpState::ACTIVE;
}
bool PowerUp::exists() const
{
    return state != PowerUpState::INACTIVE;
}
Rectangle PowerUp::getHitbox() const
{
    return hitbox;
}
Rectangle PowerUp::getGroundCheck() const
{
    return {
        hitbox.x,
        hitbox.y + hitbox.height,
        hitbox.width,
        3.0f
    };
}
Rectangle PowerUp::getLeftCheck() const
{
    return {
        hitbox.x - 2.0f,
        hitbox.y + 4.0f,
        2.0f,
        hitbox.height - 8.0f
    };
}
Rectangle PowerUp::getRightCheck() const
{
    return {
        hitbox.x + hitbox.width,
        hitbox.y + 4.0f,
        2.0f,
        hitbox.height - 8.0f
    };
}
PowerUpState PowerUp::getState() const
{
    return state;
}
PowerUpType PowerUp::getType() const
{
    return type;
}
