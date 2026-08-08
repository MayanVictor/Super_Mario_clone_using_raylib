#include<raylib.h>
#include<raymath.h>
#include <vector>
#include "playerMario.hpp"
#include "PowerUps.hpp"
#include "Blocks.hpp"
#include "Enemys.hpp"

int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    std::vector<Block> blocks;
    std::vector<PowerUp> powerUps;
    std::vector<Enemy> enemies;
    blocks.push_back(Block({200, 300, 50, 50}, SOLID));
    blocks.push_back(Block({300, 300, 50, 50}, BRICK));
    blocks.push_back(Block({400, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    blocks.push_back(Block({500, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    enemies.push_back(Enemy(EnemyType::GOOMBA, {600, 300}));
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

        Rectangle previousHitbox = mario.getHitbox(); // Store the previous hitbox before updating Mario's position

        mario.update(deltaTime); // Update Mario's position and velocity
        for(PowerUp& powerUp : powerUps) {    
            powerUp.update(deltaTime); // Update power-up positions
        }
        
        Rectangle groundCheck = {mario.getHitbox().x, mario.getHitbox().y + mario.getHitbox().height, mario.getHitbox().width, 3};
        Rectangle HeadCheck = {mario.getHitbox().x + 5, mario.getHitbox().y - 3, mario.getHitbox().width - 10, 3 };
        Rectangle leftcheck = {mario.getHitbox().x - 3, mario.getHitbox().y + 5, 3, mario.getHitbox().height - 10};
        Rectangle rightcheck = {mario.getHitbox().x + mario.getHitbox().width, mario.getHitbox().y + 5, 3, mario.getHitbox().height - 10};

        // Collision Detection

        if(CheckCollisionRecs(groundCheck, floor) && mario.isFalling()) { // Check if Mario is falling and collides with the floor
            mario.landOn(floor.y);
        }
        for(Block& block : blocks) {    // Check for collision between Mario and blocks
            Rectangle currentHitbox = mario.getHitbox();
            bool wasAbove =
            previousHitbox.y + previousHitbox.height <= block.getHitbox().y;

            bool isNowTouching =
                CheckCollisionRecs(currentHitbox, block.getHitbox());
            if(block.isDestroyed())
            {
                    continue;
            }
            else if(isNowTouching && wasAbove && mario.isFalling())
            {
                mario.landOn(block.getHitbox().y);
            }
        }
        for(Block& block : blocks) { // Check for collision between Mario and blocks
            if(block.isDestroyed())
            {
                    continue;
            }
            else if(mario.headCollision(HeadCheck, block.getHitbox()) && !mario.isFalling()) {
                mario.hitCeiling(block.getHitbox().y + block.getHitbox().height);
                if(block.getType() == BRICK && mario.getState() != MarioState::SMALL) { // Check if Mario is BIG or FIRE to break the brick
                    block.BrickDestroyed();
                }
                BlockContent content = block.activate();
                if(content == BlockContent::COIN) {
                    mario.addCoin();
                }
                if(content == BlockContent::POWERUP)
                {
                    PowerUpType type;

                    if(mario.getState() == MarioState::SMALL)
                    {
                        type = PowerUpType::MUSHROOM;
                    }
                    else
                    {
                        type = PowerUpType::FIRE_FLOWER;
                    }

                    Vector2 spawnPosition = {
                        block.getHitbox().x,
                        block.getHitbox().y
                    };


                    
                    powerUps.push_back(PowerUp(type, spawnPosition));

                    powerUps.back().generate(spawnPosition);
                }
            }

            if(CheckCollisionRecs(leftcheck, block.getHitbox()))
            {
                mario.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
            }

            if(CheckCollisionRecs(rightcheck, block.getHitbox()))
            {
                mario.hitWallOnRight(block.getHitbox().x);
            }
        }

        for(PowerUp& powerUp : powerUps)
        {
            if(CheckCollisionRecs(mario.getHitbox(), powerUp.getHitbox()) && powerUp.isActive())
            {
                if(powerUp.getType() == PowerUpType::MUSHROOM)
                {
                    mario.MarioGrowth();
                }
                if(powerUp.getType() == PowerUpType::FIRE_FLOWER)
                {
                    mario.MarioFire();
                }

                powerUp.collect();
            }

            if(powerUp.getState() == PowerUpState::EMMERGING)
            {
                continue;
            }
            if(CheckCollisionRecs(powerUp.getGroundCheck(),floor))
            {
                powerUp.landOn(floor.y);
            }

            for(Block& block : blocks) // Check for collision between power-ups and blocks
            {
                if(CheckCollisionRecs(powerUp.getGroundCheck(), block.getHitbox()))
                {
                    powerUp.landOn(block.getHitbox().y);
                }

                if(CheckCollisionRecs(powerUp.getLeftCheck(), block.getHitbox()))
                {
                    powerUp.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
                }

                if(CheckCollisionRecs(powerUp.getRightCheck(), block.getHitbox()))
                {
                    powerUp.hitWallOnRight(block.getHitbox().x);
                }
            }
            
        }
        if(mario.getCoins() >= 100) {
            mario.gainlife();
            mario.resetCoins();
        }
        
        // Draw

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawText(TextFormat("Coins: %d", mario.getCoins()), 10, 10, 20, RAYWHITE);
        DrawRectangleRec(floor, BROWN);
        mario.draw();
        for(PowerUp& powerUp : powerUps) {
            powerUp.draw();
        }
        for(Block& block : blocks) {
            block.draw();
        }
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
