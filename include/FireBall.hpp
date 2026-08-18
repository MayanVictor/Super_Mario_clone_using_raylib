#pragma once
#include <raylib.h>

enum class ShootDirection { LEFT, RIGHT };

class FireBall 
{
    public:
        FireBall(Vector2 startposition, ShootDirection direction);
        void update(float deltatime);
        void draw();
        void Bounce();
        Rectangle getHitbox();
        Rectangle getGroundCheck();
        Rectangle getFrontCheck();
        Rectangle getBackCheck();
        bool isActive();
        void destroy();
    private:
        Vector2 position;
        Vector2 velocity;
        Rectangle hitbox;
        float dirMultiplicator;
        float gravity;
        float bounce;

        bool active;
};