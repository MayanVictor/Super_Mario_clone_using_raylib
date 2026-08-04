#include<raylib.h>
#include<raymath.h>
#include <vector>
#include "playerMario.hpp"
#include "Blocks.hpp"


int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    std::vector<Block> blocks;
    Rectangle floor = {0, 475, screenWidth, 100};
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

        Rectangle groundCheck = {mario.getHitbox().x, mario.getHitbox().y + mario.getHitbox().height, mario.getHitbox().width, 3};
        Rectangle HeadCheck = {mario.getHitbox().x + 5, mario.getHitbox().y - 3, mario.getHitbox().width - 10, 3 };
        Rectangle leftcheck = {mario.getHitbox().x - 3, mario.getHitbox().y + 5, 3, mario.getHitbox().height - 10};
        Rectangle rightcheck = {mario.getHitbox().width, mario.getHitbox().y + 5, 3, mario.getHitbox().height - 10};

        if(CheckCollisionRecs(groundCheck, floor) && mario.isFalling()) {
            mario.landOn(floor.y);
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawRectangleRec(floor, BROWN);
        mario.draw();
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
