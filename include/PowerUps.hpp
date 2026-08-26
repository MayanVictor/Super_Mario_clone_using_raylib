#pragma once
#include <raylib.h>

enum class PowerUpType { NONE, MUSHROOM, FIRE_FLOWER, STAR };
enum class PowerUpState { INACTIVE, ACTIVE, EMMERGING };

class PowerUp
{
public:
    PowerUp(PowerUpType type, Vector2 position);

    void update(float deltaTime);
    void generate(Vector2 blockPosition);
    void draw();
    void collect();
    void Bounce();

    void landOn(float y);
    void hitWallOnRight(float blockLeft);
    void hitWallOnLeft(float blockRight);
    void invertDirection();
    
    Rectangle getHitbox() const;
    PowerUpType getType() const;
    PowerUpState getState() const;

    Rectangle getGroundCheck() const;
    Rectangle getLeftCheck() const;
    Rectangle getRightCheck() const;

    bool exists() const;
    bool isActive() const;
    bool isFalling() const;

private:
    PowerUpType type;
    PowerUpState state;

    Vector2 position;
    Vector2 velocity;

    Rectangle hitbox;

    float gravity;
    float emergeSpeed;
    float targetY;
};