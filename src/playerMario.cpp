#include "playerMario.hpp"
#include <raylib.h>
#include <raymath.h>

Mario::Mario() 
{
    lifes = 3;
    coins = 0;
    position = {0.0f, 0.0f};
    velocity = { 0.0f, 0.0f };
    acceleration = 900.0f;
    Hitbox = { position.x, position.y, 50.0f, 50.0f };
    gravity = 1400.0f;
    fallGravityMultiplier = 1.5f;
    maxMoveSpeed = 200.0f;
    jumpSpeed = 850.0f;
    groundFriction = 1100.0f;
    maxFallSpeed = 700.0f;
    direction = Direction::NONE;
    action = MarioAction::NORMAL;
    facingdirection = Direction::RIGHT;
    inGround = true;
    isGrowth = false;
    starPower = false;
    wasHit = false;
    starTimer = 0.0f;
    starDuration = 10.0f;
    invencibilityTimer = 0.0f;
    state = MarioState::SMALL;
}
void Mario::update(float deltaTime)
{
    if(action == MarioAction::ENTERING_PIPE)
    {
        position.y += 80.0f * deltaTime;

        Hitbox.x = position.x;
        Hitbox.y = position.y;

        return;
    }
    if(action == MarioAction::ENTERING_PIPE_RIGHT)
    {
        position.x += 40.0f * deltaTime;

        Hitbox.x = position.x;
        Hitbox.y = position.y;

        return;
    }
    if(action == MarioAction::EXITING_PIPE)
    {
        position.y -= 80.0f * deltaTime;

        Hitbox.x = position.x;
        Hitbox.y = position.y;

        return;
    }
    if(direction == Direction::LEFT)
    {
        velocity.x -= acceleration * deltaTime;
    }
    else if(direction == Direction::RIGHT)
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
    if(wasHit)
    {
        invencibilityTimer += deltaTime;
        if(invencibilityTimer >= 1.5f)
        {
            wasHit = false;
            invencibilityTimer = 0.0f;
        }
    }

    if(state == MarioState::BIG || state == MarioState::FIRE) {
        Hitbox.height = 80;
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
    if(velocity.y < -500.0f)
    {
        velocity.y = -500.0f;
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
void Mario::isHit()
{
    wasHit = true;
}
void Mario::setDirection(Direction dir)
{
    direction = dir;

    if(dir == Direction::LEFT || dir == Direction::RIGHT)
    {
        facingdirection = dir;
    }

}
void Mario::setMarioAction(MarioAction Action)
{
    action = Action;
}
Direction Mario::getFacingDirection() const
{
    return facingdirection;
}
MarioAction Mario::getMarioAction() const
{
    return action;
}
Rectangle Mario::getHitbox()
{
    return Hitbox;
}
bool Mario::getStarPower() const
{
    return starPower;
}
bool Mario::isInvincible() const
{
    return wasHit;
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
    Hitbox.height = 80;
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
void Mario::setState(MarioState newState)
{
    state = newState;
}
void Mario::setPosition(Vector2 pos)
{
    position = pos;
    Hitbox.x = position.x;
    Hitbox.y = position.y;
}
void Mario::draw()
{
    Rectangle Body = Hitbox;
    Rectangle Face;

    /**if(state == MarioState::SMALL) {
        Face = {Hitbox.x, Hitbox.y + 10, Hitbox.width , 10};
        DrawRectangleRec(Body, RED);
        DrawRectangleRec(Face, BEIGE);
    } else if(state == MarioState::BIG) {
        Face = {Hitbox.x, Hitbox.y + 15, Hitbox.width, 25};
        DrawRectangleRec(Body, RED);
        DrawRectangleRec(Face, BEIGE);
    } else if(state == MarioState::FIRE) {
        Face = {Hitbox.x, Hitbox.y + 15, Hitbox.width , 25};
        DrawRectangleRec(Body, ORANGE);
        DrawRectangleRec(Face, BEIGE);
    }**/
    switch(state)
    {
        case(MarioState::SMALL) :
            Face = {Hitbox.x, Hitbox.y + 10, Hitbox.width , 10};
            DrawRectangleRec(Body, RED);
            DrawRectangleRec(Face, BEIGE);
            break;
        case(MarioState::BIG) :
            Face = {Hitbox.x, Hitbox.y + 15, Hitbox.width, 25};
            DrawRectangleRec(Body, RED);
            DrawRectangleRec(Face, BEIGE);
            break;
        case(MarioState::FIRE) :
            Face = {Hitbox.x, Hitbox.y + 15, Hitbox.width , 25};
            DrawRectangleRec(Body, ORANGE);
            DrawRectangleRec(Face, BEIGE);
            break;
        default :
            break;
    }
    if(starPower)
    {
        int blink = (int)(starTimer * 12.0f);

        if(blink % 2 == 0)
        {
            DrawRectangleRec(Hitbox, RAYWHITE);
        }
    }
    if(wasHit)
    {
        int shading = (int)(invencibilityTimer * 6.0f);
        if(shading % 2 == 0)
        {
            DrawRectangleRec(Hitbox, {245, 245, 245, 100});
        }
    }
}