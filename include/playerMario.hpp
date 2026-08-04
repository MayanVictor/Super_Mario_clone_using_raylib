#pragma once
#include <raylib.h>

enum Direction { LEFT, RIGHT, NONE };
class Mario {
    public:
        Mario();
        void update(float deltaTime);
        void jump();
        void stopJump();
        Direction setDirection(Direction dir);
        Rectangle getHitbox();
        bool IsInGround(bool inground);
        bool headCollision(Rectangle head, Rectangle Block);
        bool getGrowth() const ;
        void MarioGrowth();
        void collideFromLeft(float blockLeft);
        void collideFromRight(float blockRight);
        void landOn(float floorY);
        bool isFalling() const;
        void draw();
    private:
        Vector2 position;
        Vector2 velocity;
        Rectangle Hitbox;
        Direction direction;
        int health;
        float gravity;
        float groundFriction;
        float fallGravityMultiplier;
        float maxMoveSpeed;
        float acceleration;
        float jumpSpeed ;
        float maxFallSpeed;
        bool inGround;
        bool isGrowth;
};