#pragma once 
#include <raylib.h>

enum class EnemyType { GOOMBA, KOOPA, PIRANHA_PLANT };
class Enemy {
    public:
        Enemy(EnemyType type, Vector2 position);
        void update(float deltaTime);
        void draw();
        Rectangle getHitbox() const;
        EnemyType getType() const;
        bool isAlive() const;
        float getVelocityX() const;

        void hitWallOnRight(float blockLeft);
        void hitWallOnLeft(float blockRight);
        void LandOn(float floorY);
        void invertDirection();
        void setX(float x);
        
        Rectangle getGroundCheck() const;
        Rectangle getLeftCheck() const;
        Rectangle getRightCheck() const;
        void defeat();
    private:
        EnemyType type;
        Vector2 position;
        Vector2 velocity;
        Rectangle hitbox;
        bool alive;
        float gravity;
        float maxFallSpeed;
};