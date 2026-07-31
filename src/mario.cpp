#include<raylib.h>
#include<raymath.h>
#include <vector>

enum Direction { LEFT, RIGHT, NONE };
class Mario {
    public:
        Mario() {
            health = 1;
            position = { 100.0f, 425.0f };
            velocity = { 0.0f, 0.0f };
            acceleration = 900.0f;
            Hitbox = { position.x, position.y, 50.0f, 50.0f };
            gravity = 1400.0f;
            fallGravityMultiplier = 1.5f;
            maxMoveSpeed = 200.0f;
            jumpSpeed = 520.0f;
            groundFriction = 1100.0f;
            maxFallSpeed = 700.0f;
            direction = NONE;
            inGround = true;
        }
        void update(float deltaTime) {
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
        }
        void jump() {
            if(inGround) {
                velocity.y = -jumpSpeed;
                inGround = false;
            }
        }
        void stopJump()
        {
            if(velocity.y < -150.0f)
            {
                velocity.y = -150.0f;
            }
        }
        Direction setDirection(Direction dir) {
            direction = dir;
            return direction;
        }
        Rectangle getHitbox() {
            return Hitbox;
        }
        bool IsInGround(bool inground) {
            inGround = inground;
            return inGround;
        }
        void landOn(float floorY)
        {
            position.y = floorY - Hitbox.height;
            velocity.y = 0.0f;
            inGround = true;

            Hitbox.y = position.y;
        }
        bool isFalling() const {
            return velocity.y >= 0.0f;
        }
        void draw() {
            DrawRectangleRec(Hitbox, RED);
        }
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
};

int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    Rectangle floor = {0, 475, screenWidth, 100};
    Rectangle groundCheck = mario.getHitbox();
    groundCheck.y += 2.0f;
    InitWindow(screenWidth, screenHeight, "Mario Game");

    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        //input
        if(IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
            mario.setDirection(RIGHT);
        }
        else if(IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
            mario.setDirection(LEFT);
        }
        else {
            mario.setDirection(NONE);
        }
        if(IsKeyPressed(KEY_UP)) {
            mario.jump();
        }
        if(IsKeyReleased(KEY_UP)) {
            mario.stopJump();
        }
        
        
        // Update
        mario.update(deltaTime);
        if(CheckCollisionRecs(groundCheck, floor) && mario.isFalling()) {
            mario.landOn(floor.y);
        }
    
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(floor, BROWN);
        mario.draw();
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
