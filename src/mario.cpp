#include<raylib.h>
#include<raymath.h>
#include <vector>
#include "playerMario.hpp"
#include "PowerUps.hpp"
#include "Blocks.hpp"
#include "Enemy.hpp"


int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    std::vector<Block> blocks;
    std::vector<PowerUp> powerUps;
    std::vector<Enemy> enemies;
    blocks.push_back(Block({200, 300, 50, 50}, SOLID));
    blocks.push_back(Block({200, 425, 50, 50}, SOLID));
    blocks.push_back(Block({300, 300, 50, 50}, BRICK));
    blocks.push_back(Block({400, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    blocks.push_back(Block({500, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    enemies.push_back(Enemy(EnemyType::GOOMBA, {600, 300}));
    enemies.push_back(Enemy(EnemyType::KOOPA, {700, 300}));
    
    Rectangle floor = {0, 475, screenWidth + 100, 125};
    float cameraThreshold = screenWidth * 0.45f;

    Camera2D camera = { 0 };
    camera.target = { screenWidth/2.0f, screenHeight/2.0f };
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    InitWindow(screenWidth, screenHeight, "Mario Game");

    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Vector2 marioPosition = mario.getPosition();
        float marioScreenX = marioPosition.x - camera.target.x + camera.offset.x;
        Rectangle backWall = {camera.target.x - camera.offset.x - 10, 0, 10, (float)screenHeight};
        // =========
        //   input
        // =========
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
        
        // ==========
        //   Update
        // ==========

        Rectangle previousHitbox = mario.getHitbox(); // Store the previous hitbox before updating Mario's position

        mario.update(deltaTime); // Update Mario's position and velocity
        for(PowerUp& powerUp : powerUps) {    
            powerUp.update(deltaTime); // Update power-up positions
        }
        for(Enemy& enemy : enemies) {    
            if(enemy.isAlive()) {
                enemy.update(deltaTime); // Update enemy positions
            }
        }
        if(marioScreenX > cameraThreshold) {
            camera.target.x += marioScreenX - cameraThreshold;
        }
        

        // ===================
        // Collision Detection
        // ===================

        if(CheckCollisionRecs(mario.getGroundCheck(), floor) && mario.isFalling()) { // Check if Mario is falling and collides with the floor
            mario.landOn(floor.y);
        }
        if(CheckCollisionRecs(mario.getLeftCheck(), backWall)
            && mario.getVelocityX() < 0.0f)
            {
                mario.hitWallOnLeft(backWall.x + backWall.width);
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
            else if(CheckCollisionRecs(mario.getHeadCheck(), block.getHitbox()) && !mario.isFalling()) {
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

            if(CheckCollisionRecs(mario.getLeftCheck(), block.getHitbox()))
            {
                mario.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
            }

            if(CheckCollisionRecs(mario.getRightCheck(), block.getHitbox()))
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

        for(Enemy& enemy : enemies)
        {
            if(!enemy.isAlive())
            {
                continue;
            }
            if(enemy.getHitbox().x + enemy.getHitbox().width <= 0) {
                enemy.defeat();
            }
            if(CheckCollisionRecs(mario.getGroundCheck(), enemy.getHitbox()) && mario.isFalling()) // Check if Mario is above the enemy
            {
                enemy.defeat();
                mario.bounce();
            }
            else if(CheckCollisionRecs(mario.getHitbox(), enemy.getHitbox())) // Check if Mario collides with the enemy
            {
                if(mario.getState() == MarioState::FIRE || mario.getState() == MarioState::BIG)
                {
                    mario.setState(MarioState::SMALL);
                }
                else if(mario.getState() == MarioState::SMALL)
                {
                        mario.loselife();
                }
            }

            if(CheckCollisionRecs(enemy.getGroundCheck(),floor))
            {
                enemy.LandOn(floor.y);
            }
           

            for(Block& block : blocks) // Check for collision between blocks
            {
                if(block.isDestroyed()) continue;
                if(CheckCollisionRecs(enemy.getGroundCheck(), block.getHitbox()))
                {
                    enemy.LandOn(block.getHitbox().y);
                }

                if(enemy.getVelocityX() < 0.0f && CheckCollisionRecs(enemy.getLeftCheck(), block.getHitbox()))
                {
                    enemy.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
                }
                else if(enemy.getVelocityX() > 0.0f && CheckCollisionRecs(enemy.getRightCheck(), block.getHitbox()))
                {
                    enemy.hitWallOnRight(block.getHitbox().x);
                }
            }

        }
        for(size_t i = 0; i < enemies.size(); i++) // Check collision between enemies and change their directions
            {
                if(!enemies[i].isAlive()) continue;

                for(size_t j = i + 1; j < enemies.size(); j++)
                {
                    if(!enemies[j].isAlive()) continue;

                    Enemy& enemyA = enemies[i];
                    Enemy& enemyB = enemies [j];

                    Rectangle a = enemyA.getHitbox();
                    Rectangle b = enemyB.getHitbox();

                    if(CheckCollisionRecs(a, b))
                    {
                        float centerA = a.x + a.width / 2.0f;
                        float centerB = b.x + b.width / 2.0f;

                        float overlap;

                        if(centerA < centerB)
                        {
                            // A is on left
                            overlap = (a.x + a.width) - b.x;

                            enemyA.setX(a.x - overlap / 2.0f);
                            enemyB.setX(b.x + overlap / 2.0f);
                        }
                        else
                        {
                            // B is on left
                            overlap = (b.x + b.width) - a.x;

                            enemyB.setX(b.x - overlap / 2.0f);
                            enemyA.setX(a.x + overlap / 2.0f);
                        }
                        
                        enemyA.invertDirection();
                        enemyB.invertDirection();
                    }
                }
            }
        if(mario.getCoins() >= 100) {
            mario.gainlife();
            mario.resetCoins();
        }
        
        // ======
        //  Draw
        // ======

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode2D(camera);
            DrawRectangleRec(floor, BROWN);
            mario.draw();
            for(PowerUp& powerUp : powerUps) powerUp.draw();
            for(Block& block : blocks) block.draw();
            for(Enemy& enemy : enemies) enemy.draw();
        EndMode2D();

        DrawText(TextFormat("Coins: %d", mario.getCoins()), 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
