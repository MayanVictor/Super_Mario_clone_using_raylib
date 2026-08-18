#include<raylib.h>
#include<raymath.h>
#include <vector>
#include "playerMario.hpp"
#include "PowerUps.hpp"
#include "Blocks.hpp"
#include "Enemy.hpp"
#include "FireBall.hpp"
#include "Shell.hpp"


int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    std::vector<Block> blocks;
    std::vector<PowerUp> powerUps;
    std::vector<Enemy> enemies;
    std::vector<FireBall> fireballs;
    std::vector<Shell> shells;
    blocks.push_back(Block({200, 300, 50, 50}, SOLID));
    blocks.push_back(Block({300, 300, 50, 50}, BRICK));
    blocks.push_back(Block({400, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    blocks.push_back(Block({500, 300, 50, 50}, QUESTION, BlockContent::POWERUP));
    blocks.push_back(Block({800, 300, 50, 50}, QUESTION, BlockContent::STAR));
    enemies.push_back(Enemy(EnemyType::GOOMBA, {1500, 420}));
    enemies.push_back(Enemy(EnemyType::KOOPA, {1600, 420}));
    
    Rectangle floor = {0, 475, 2 * screenWidth, 125};
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
        float cameraLeft =
            camera.target.x - camera.offset.x;

        float cameraRight =
            cameraLeft + screenWidth;
        Rectangle cameraArea = {
            cameraLeft - 100.0f,
            camera.target.y - camera.offset.y,
            (float)screenWidth + 100.0f,
            (float)screenHeight
        };
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
        if(IsKeyDown(KEY_LEFT_SHIFT)) {
            mario.SpeedUp();
        }
        if(IsKeyReleased(KEY_LEFT_SHIFT)) {
            mario.SpeedDown();
        }
        if(mario.getState() == MarioState::FIRE && IsKeyPressed(KEY_Q)){
            if(mario.getFacingDirection() == Direction::RIGHT) 
                fireballs.push_back(FireBall(mario.getPosition(), ShootDirection::RIGHT));
            if(mario.getFacingDirection() == Direction::LEFT) fireballs.push_back(FireBall(mario.getPosition(), ShootDirection::LEFT));
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
            if(enemy.isAlive() && enemy.isActive()) {
                enemy.update(deltaTime); // Update enemy positions
            }
        }
        for(Shell& shell : shells) {
            shell.update(deltaTime);
        }
        for(FireBall& fireball : fireballs) {
            fireball.update(deltaTime);
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
                if(content == BlockContent::STAR)
                {
                    PowerUpType type = PowerUpType::STAR;
                    Vector2 spawnPosition = {
                        block.getHitbox().x,
                        block.getHitbox().y};

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
                if(powerUp.getType() == PowerUpType::STAR)
                {
                    mario.obtainStar();
                }

                powerUp.collect();
            }

            if(powerUp.getState() == PowerUpState::EMMERGING)
            {
                continue;
            }
            if(powerUp.isFalling() && CheckCollisionRecs(powerUp.getGroundCheck(),floor))
            {
                powerUp.landOn(floor.y);
            }

            for(Block& block : blocks) // Check for collision between power-ups and blocks
            {
                if(powerUp.isFalling() && CheckCollisionRecs(powerUp.getGroundCheck(), block.getHitbox()))
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
            if(CheckCollisionRecs(enemy.getHitbox(), cameraArea)) {
                enemy.setActive(true);
            }
            if(enemy.getHitbox().x + enemy.getHitbox().width <= 0) {
                enemy.defeat();
            }
            if(mario.getStarPower())
            {
                if(CheckCollisionRecs(mario.getHitbox(), enemy.getHitbox())) enemy.defeat();
            } else {
                if(CheckCollisionRecs(mario.getGroundCheck(), enemy.getHitbox()) && mario.isFalling()) // Check if Mario is above the enemy
                {
                    enemy.defeat();
                    mario.bounce();
                    if(enemy.getType() == EnemyType::KOOPA)
                    {
                        shells.push_back(Shell({enemy.getHitbox().x, enemy.getHitbox().y + 20}));
                    }
                }
                else if(CheckCollisionRecs(mario.getLeftCheck(), enemy.getHitbox())
                        || CheckCollisionRecs(mario.getRightCheck(), enemy.getHitbox())
                        || CheckCollisionRecs(mario.getHeadCheck(), enemy.getHitbox())) // Check if Mario collides with the enemy
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

        for(Shell& shell : shells)
        {
            if(!shell.isExisting()) continue;

            if(CheckCollisionRecs(shell.getGroundCheck(), floor) && shell.isFalling()) // shell on floor
            {
                shell.LandOn(floor.y);
            }
            if(CheckCollisionRecs(mario.getHitbox(), shell.getHitBox())) 
            {
                if(mario.getStarPower())
                {
                    if(CheckCollisionRecs(mario.getHitbox(), shell.getHitBox()))
                        shell.destroy();
                }
                else {
                    if(!shell.isMoving())
                    {
                        if(CheckCollisionRecs(mario.getGroundCheck(),shell.getTopCheck()) && mario.isFalling())
                        {
                            mario.bounce();
                        }
                        else 
                        {
                            float marioCenter =
                                mario.getHitbox().x +
                                mario.getHitbox().width / 2.0f;
                            float shellCenter =
                                shell.getHitBox().x +
                                shell.getHitBox().width / 2.0f;
                            if(marioCenter < shellCenter)
                            {
                                shell.Kick(ShellDirection::RIGHT);
                            }
                            else {
                                shell.Kick(ShellDirection::LEFT);
                            }
                        }
                    }
                    else {
                        if(CheckCollisionRecs(mario.getGroundCheck(), shell.getTopCheck()))
                        {
                            shell.Stop();
                            mario.bounce();
                        }
                        else if(shell.canDamageMario()) {
                            if(mario.getState() == MarioState::BIG || mario.getState() == MarioState::FIRE)
                            {
                                mario.setState(MarioState::SMALL);
                            }
                            else 
                            {
                                mario.loselife();
                            }
                        }  
                    }
                }
            }
            for(Enemy& enemy : enemies)
            {
                if(CheckCollisionRecs(enemy.getHitbox(), shell.getHitBox()) && shell.isMoving())
                {
                    enemy.defeat();
                }
            }
            for(Block& block : blocks)
            {
                if(CheckCollisionRecs(shell.getGroundCheck(), block.getHitbox()) && shell.isFalling())
                {
                    shell.LandOn(block.getHitbox().y);
                }
                if(shell.isMoving()) {
                    if(CheckCollisionRecs(shell.getRightCheck(), block.getHitbox()) 
                        || CheckCollisionRecs(shell.getLeftCheck(), block.getHitbox()))
                    {
                        shell.invertDirection();
                    }
                }
            }
        }
        for(FireBall& fireball : fireballs) 
        {
            if(!fireball.isActive()) continue;

            if(fireball.getHitbox().x > cameraArea.x + cameraArea.width 
                || fireball.getHitbox().x < cameraArea.x) 
                {
                    fireball.destroy();
                }

            if(CheckCollisionRecs(fireball.getGroundCheck(), floor)) 
            {
                fireball.Bounce();
            }
            for(Enemy& enemy : enemies) 
            {
                if(CheckCollisionRecs(fireball.getHitbox(), enemy.getHitbox()))
                {
                    fireball.destroy();
                    enemy.defeat();
                }
            }
            for(Block& block : blocks) 
            {
                if(CheckCollisionRecs(fireball.getGroundCheck(), block.getHitbox()))
                {
                    fireball.Bounce();
                }
                if(CheckCollisionRecs(fireball.getFrontCheck(), block.getHitbox()))
                {
                    fireball.destroy();
                }
                if(CheckCollisionRecs(fireball.getBackCheck(), block.getHitbox()))
                {
                    fireball.destroy();
                }
            }
            for(Shell& shell : shells)
            {
                if(CheckCollisionRecs(fireball.getHitbox(), shell.getHitBox()))
                {
                    fireball.destroy();
                    shell.destroy();
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
            for(Shell& shell : shells) shell.draw();
            for(FireBall& fireball : fireballs) fireball.draw();
        EndMode2D();

        DrawText(TextFormat("Coins: %d", mario.getCoins()), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Lifes: %d", mario.getLifes()), 10, 40, 20, RAYWHITE);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
