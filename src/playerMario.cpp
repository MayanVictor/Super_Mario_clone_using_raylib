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
    facingdirection = RIGHT;
    inGround = true;
    isGrowth = false;
    starPower = false;
    starTimer = 0.0f;
    starDuration = 10.0f;
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

    if(starPower)
    {
        starTimer += deltaTime;

        if(starTimer >= starDuration)
        {
            starPower = false;
            starTimer = 0.0f;
        }
    }

    if(state == MarioState::BIG || state == MarioState::FIRE) {
        Hitbox.height = 100;
    } else 
    {
        Hitbox.height = 50;
        isGrowth = false;
    }
}
void Mario::jump()
{
    if(inGround) 
    {
        velocity.y = -jumpSpeed;
        inGround = false;
    }
}
void Mario::bounce()
{
    velocity.y = -jumpSpeed * 0.25f; // Bounce with 25% of the jump speed
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
void Mario::setDirection(Direction dir)
{
    direction = dir;

    if(dir == LEFT || dir == RIGHT)
    {
        facingdirection = dir;
    }

}
Direction Mario::getFacingDirection() const
{
    return facingdirection;
}
Rectangle Mario::getHitbox()
{
    return Hitbox;
}
bool Mario::getStarPower() const
{
    return starPower;
}
void Mario::obtainStar()
{
    starPower = true;
}
void Mario::SpeedUp()
{
    maxMoveSpeed = 350;
}
void Mario::SpeedDown()
{
    maxMoveSpeed = 200;
}
Vector2 Mario::getPosition() {
    return position;
}
Rectangle Mario::getGroundCheck() const
{
    return {
        Hitbox.x,
        Hitbox.y + Hitbox.height,
        Hitbox.width,
        3.0f
    };
}
Rectangle Mario::getHeadCheck() const
{
    return {
        Hitbox.x + 5.0f,
        Hitbox.y - 3.0f,
        Hitbox.width - 10.0f,
        3.0f
    };
}
Rectangle Mario::getLeftCheck() const
{
    return {
        Hitbox.x - 3.0f,
        Hitbox.y + 5.0f,
        3.0f,
        Hitbox.height - 10.0f
    };
}
Rectangle Mario::getRightCheck() const
{
    return {
        Hitbox.x + Hitbox.width,
        Hitbox.y + 5.0f,
        3.0f,
        Hitbox.height - 10.0f
    };
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
float Mario::getVelocityX() const
{
    return velocity.x;
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
MarioState Mario::setState(MarioState newState)
{
    state = newState;
    return state;
}
void Mario::draw()
{
    if(state == MarioState::SMALL) {
        DrawRectangleRec(Hitbox, RED);
    } else if(state == MarioState::BIG) {
        DrawRectangleRec(Hitbox, RED);
    } else if(state == MarioState::FIRE) {
        DrawRectangleRec(Hitbox, ORANGE);
    }
    if(starPower)
    {
        int blink = (int)(starTimer * 12.0f);

        if(blink % 2 == 0)
        {
            DrawRectangleRec(Hitbox, RAYWHITE);
        }
    }
}