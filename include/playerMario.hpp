#pragma once
#include <raylib.h>

enum Direction { LEFT, RIGHT, NONE };
enum MarioState { SMALL, BIG, FIRE };
class Mario {
    public:
        Mario();
        void update(float deltaTime);
        void jump();
        void stopJump();
        void bounce();
        void hitCeiling(float blockBottom);
        Direction setDirection(Direction dir);
        Rectangle getHitbox();
        Rectangle getGroundCheck() const;
        Rectangle getHeadCheck() const; 
        Rectangle getLeftCheck() const;
        Rectangle getRightCheck() const;
        MarioState setState(MarioState newState);
        MarioState getState() const ;
        Vector2 getPosition();
        void MarioGrowth();
        void MarioFire();
        void hitWallOnLeft(float blockRight);
        void hitWallOnRight(float blockLeft);
        void landOn(float floorY);
        void loselife();
        void gainlife();
        void addCoin();
        int getLifes() const;
        int getCoins() const;
        void resetCoins();
        bool isFalling() const;
        void draw();
    private:
        Vector2 position;
        Vector2 velocity;
        Rectangle Hitbox;
        Direction direction;
        MarioState state;
        int lifes;
        int coins;
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