#pragma once
#include <raylib.h>

enum class ShellState { IDLE, MOVING };
enum class ShellDirection { RIGHT, NONE ,LEFT };

class Shell
{
    public:
        Shell(Vector2 inicialPostion);
        void Kick(ShellDirection dir);
        void Stop();
        void update(float deltatime);
        void draw();
        void LandOn(float floorY);
        bool isMoving() const;
        bool isExisting() const;
        bool isFalling() const;
        bool canDamageMario() const;
        void destroy();
        void invertDirection();
        void hitWallOnLeft(float blockLeft);
        void hitWallOnRight(float blockRight);
        float getVelocityX();
        ShellState getShellState() const;
        Rectangle getHitBox();
        Rectangle getGroundCheck();
        Rectangle getRightCheck();
        Rectangle getLeftCheck();
        Rectangle getTopCheck();
    private:
        Vector2 position;
        Vector2 velocity;
        Rectangle hitbox;
        ShellState state;
        ShellDirection direction;
        float gravity;
        float kickcooldown;

        bool exist;
        bool moving;
};  
