#include<raylib.h>
#include<raymath.h>
#include <vector>
#include "playerMario.hpp"
#include "PowerUps.hpp"
#include "Blocks.hpp"
#include "Enemy.hpp"
#include "FireBall.hpp"
#include "Shell.hpp"
#include "Pipe.hpp"
#include "Coin.hpp"
#include "Flag.hpp"
#include <string>
#include <fstream>

enum class DescorationType{PIPE_VERTICAL};

struct Decoration
{
    Rectangle rect;
    DescorationType type;
};
struct Area 
    {
        std::vector<Enemy> enemies;
        std::vector<Block> blocks;
        std::vector<Coin> coins;
        std::vector<Pipe> pipes;
        std::vector<Shell> shells;
        std::vector<FireBall> fireballs;
        std::vector<PowerUp> powerUps;
        std::vector<Decoration> decorations;

        Vector2 spawnposition = {0, 0};

        float width = 0.0f;
        float height = 0.0f;
    };

enum class GameArea { OVER_WORLD , COIN_ROOM};

class MapLoader
{
public:

    static void loadMap(
        const std::string& filename,
        Area& area,
        Flag* flag = nullptr
    )
    {
        std::ifstream file(filename);

        if(!file.is_open())
        {
            TraceLog(
                LOG_ERROR,
                "it's not possible to load the map"
            );

            return;
        }

        const int TILE_SIZE = 50;

        std::string line;

        float sectionOffsetX = 0.0f;

        int row = 0;
        int maxColumns = 0;
        int maxRows = 0;

        float totalWidth = 0.0f;
        float maxHeight = 0.0f;

        while(std::getline(file, line))
        {
            // Nova seção do mapa
            if(line.empty())
            {
                float sectionWidth =
                    maxColumns * TILE_SIZE;

                sectionOffsetX += sectionWidth;

                totalWidth = sectionOffsetX;

                float sectionHeight =
                    maxRows * TILE_SIZE;

                if(sectionHeight > maxHeight)
                {
                    maxHeight = sectionHeight;
                }

                row = 0;
                maxColumns = 0;
                maxRows = 0;

                continue;
            }

            int column = 0;

            for(char symbol : line)
            {
                // ignora separadores do txt
                if(symbol == ' ' || symbol == '\t')
                    continue;

                float x =
                    sectionOffsetX +
                    column * TILE_SIZE;

                float y =
                    row * TILE_SIZE;

                switch(symbol)
                {
                    case '.':
                        break;

                    case '#':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE}, FLOOR));
                        break;

                    case 'M':
                        area.spawnposition = {x, y};
                        break;

                    case 'B':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE},BRICK));
                        break;

                    case 's':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE},SOLID));
                        break;

                    case 'c':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE},
                                QUESTION,BlockContent::COIN));
                        break;

                    case '?':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE},
                                QUESTION, BlockContent::POWERUP));
                        break;
                    case 'S':
                        area.blocks.push_back(Block({x, y, TILE_SIZE, TILE_SIZE}, QUESTION, BlockContent::STAR));
                        break;
                    case 'G':
                        area.enemies.push_back(Enemy(EnemyType::GOOMBA, {x, y}));
                        break;
                    case 'K':
                        area.enemies.push_back(Enemy(EnemyType::KOOPA, {x, y}));
                        break;
                    case 'P':
                        area.pipes.push_back(Pipe({x, y, 100, 800},PipeType::NORMAL));
                        break;
                    case '$':
                        area.coins.push_back(
                            Coin({x, y})
                        );
                        break;
                    case 'C':
                        area.pipes.push_back(
                            Pipe({x, y, 200, 100}, PipeType::EXIT, {124 * 50, 400}, PipeDestiny::OVER_WORLD, PipeDirection::LEFT));
                        break;
                    case 'T':
                        area.decorations.push_back(Decoration{{x, y, TILE_SIZE, TILE_SIZE}, DescorationType::PIPE_VERTICAL});
                        break;
                    case 'E':
                        area.pipes.push_back(Pipe({x, y, 100, 800}, PipeType::ENTRANCE, 
                            {125, 50}, PipeDestiny::COIN_ROOM,PipeDirection::UP));
                        break;
                    case 'b':
                        if(flag != nullptr)
                        {
                            flag->setPosition({x, y});
                        }
                        break;
                }

                column++;
            }

            if(column > maxColumns)
            {
                maxColumns = column;
            }

            row++;

            if(row > maxRows)
            {
                maxRows = row;
            }
        }

        totalWidth = sectionOffsetX + maxColumns * TILE_SIZE;

        float lastSectionHeight =
            maxRows * TILE_SIZE;

        if(lastSectionHeight > maxHeight)
        {
            maxHeight = lastSectionHeight;
        }

        area.width = totalWidth;
        area.height = maxHeight;

        file.close();
    }
};

void DrawCastle(float X);

int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;
    Mario mario;
    Flag flag;

    GameArea currentArea = GameArea::OVER_WORLD;

    Area* area = nullptr;
    Area overworld;
    Area coinRoom;

    MapLoader::loadMap("areas/overworld.txt", overworld, &flag);
    MapLoader::loadMap("areas/coinroom.txt", coinRoom);

    float cameraThreshold = screenWidth * 0.45f;
    float pipeTimer = 0.0f;
    Pipe* activePipe = nullptr;
    float exitPipeTargetY = 0.0f;

    Camera2D camera = { 0 };
    camera.target = { screenWidth/2.0f, screenHeight/2.0f };
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    int score = 0;

    mario.setPosition(overworld.spawnposition);
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

            if(currentArea == GameArea::OVER_WORLD)
            {
                area = &overworld;
            } 
            else
            {
                area = &coinRoom;
            }
            // =========
            //   input
            // =========
            if(IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
                mario.setDirection(Direction::RIGHT);
            }
            else if(IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
                mario.setDirection(Direction::LEFT);
            }
            else {
                mario.setDirection(Direction::NONE);
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
                    area->fireballs.push_back(FireBall(marioPosition, ShootDirection::RIGHT));
                if(mario.getFacingDirection() == Direction::LEFT) 
                    area->fireballs.push_back(FireBall(marioPosition, ShootDirection::LEFT));
            }
            
            // ==========
            //   Update
            // ==========

            Rectangle previousHitbox = mario.getHitbox(); // Store the previous hitbox before updating Mario's position

            mario.update(deltaTime); // Update Mario's position and velocity
            for(PowerUp& powerUp : area->powerUps) {    
                powerUp.update(deltaTime); // Update power-up positions
            }
            for(Enemy& enemy : area->enemies) {    
                if(enemy.isAlive() && enemy.isActive()) {
                    enemy.update(deltaTime); // Update enemy positions
                }
            }
            for(Shell& shell : area->shells) {
                shell.update(deltaTime);
            }
            for(FireBall& fireball : area->fireballs) {
                fireball.update(deltaTime);
            }
            if(currentArea == GameArea::OVER_WORLD) {
                if(marioScreenX > cameraThreshold) {
                    camera.target.x += marioScreenX - cameraThreshold;
                }
            } 
            else if(currentArea == GameArea::COIN_ROOM)  
            {
                camera.target = {coinRoom.width / 2, coinRoom.height / 2};
            }

            // ===================
            // Collision Detection
            // ===================

            //check mario collisions
            if(CheckCollisionRecs(mario.getLeftCheck(), backWall)
                && mario.getVelocityX() < 0.0f)
                {
                    mario.hitWallOnLeft(backWall.x + backWall.width);
                }
            for(Block& block : area->blocks) {    // Check for collision between Mario and blocks
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


                        
                        area->powerUps.push_back(PowerUp(type, spawnPosition));

                        area->powerUps.back().generate(spawnPosition);
                    }
                    if(content == BlockContent::STAR)
                    {
                        PowerUpType type = PowerUpType::STAR;
                        Vector2 spawnPosition = {
                            block.getHitbox().x,
                            block.getHitbox().y};

                        area->powerUps.push_back(PowerUp(type, spawnPosition));

                        area->powerUps.back().generate(spawnPosition);
                    }
                }

                if(CheckCollisionRecs(mario.getLeftCheck(), block.getHitbox()) && mario.getVelocityX() < 0.0f)
                {
                    mario.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
                }

                if(CheckCollisionRecs(mario.getRightCheck(), block.getHitbox()) && mario.getVelocityX() > 0.0f)
                {
                    mario.hitWallOnRight(block.getHitbox().x);
                }
            }
            for(Decoration& decoration : area->decorations)
            {
                if(CheckCollisionRecs(mario.getRightCheck(), decoration.rect) && mario.getVelocityX() > 0.0f)
                {
                    mario.hitWallOnRight(decoration.rect.x);
                }
            }

            for(PowerUp& powerUp : area->powerUps) // check the power-ups collisions
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

                for(Block& block : area->blocks) // Check for collision between power-ups and blocks
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
                    if(CheckCollisionRecs(mario.getHeadCheck(), block.getHitbox()) 
                        && CheckCollisionRecs(powerUp.getGroundCheck(), block.getHitbox()) 
                        && powerUp.getType() == PowerUpType::MUSHROOM)
                    {
                        powerUp.invertDirection();
                        powerUp.Bounce();
                    }
                }
                for(Pipe& pipe : area->pipes)
                {
                    if(CheckCollisionRecs(powerUp.getRightCheck(), pipe.getHitbox()))
                    {
                        powerUp.hitWallOnRight(pipe.getHitbox().x);
                    }
                    if(CheckCollisionRecs(powerUp.getGroundCheck(), pipe.getEntranceCheck()) 
                        && mario.getMarioAction() == MarioAction::NORMAL) 
                    {
                        powerUp.landOn(pipe.getHitbox().y);
                    }
                    if(CheckCollisionRecs(powerUp.getLeftCheck(), pipe.getHitbox()))
                    {
                        powerUp.hitWallOnRight(pipe.getHitbox().x + pipe.getHitbox().width);
                    }
                }
                
            }
            for(Pipe& pipe : area->pipes)
            {
                if(CheckCollisionRecs(mario.getRightCheck(), pipe.getHitbox()) && mario.getVelocityX() > 0.0f)
                {
                    mario.hitWallOnRight(pipe.getHitbox().x);
                }
                else if(CheckCollisionRecs(mario.getLeftCheck(), pipe.getHitbox()) && mario.getVelocityX() < 0.0f)
                {
                    mario.hitWallOnLeft(pipe.getHitbox().x + pipe.getHitbox().width);
                }
                if(pipe.getType() == PipeType::ENTRANCE ||
                    pipe.getType() == PipeType::EXIT)
                {
                    if(CheckCollisionRecs(mario.getHitbox(),pipe.getEntranceCheck()))
                        {
                            if(pipe.getDirection() == PipeDirection::UP && IsKeyDown(KEY_DOWN) 
                                && mario.getMarioAction() == MarioAction::NORMAL)
                            {
                                mario.setMarioAction(MarioAction::ENTERING_PIPE);
                                activePipe = &pipe;
                                pipeTimer = 0.0f;
                            }

                            else if(pipe.getDirection() == PipeDirection::LEFT && IsKeyDown(KEY_RIGHT) 
                                && mario.getMarioAction() == MarioAction::NORMAL)
                            {
                                mario.setMarioAction(MarioAction::ENTERING_PIPE_RIGHT);
                                activePipe = &pipe;
                                pipeTimer = 0.0f;
                            }
                           
                        }
                    }
                if(mario.getMarioAction() == MarioAction::NORMAL) 
                {

                    if(CheckCollisionRecs(mario.getGroundCheck(), pipe.getEntranceCheck()) && mario.isFalling())
                    {
                        mario.landOn(pipe.getHitbox().y);
                    }
                }
            }

            if(activePipe != nullptr && mario.getMarioAction() != MarioAction::NORMAL)
            {
                pipeTimer += deltaTime;
                if(pipeTimer >= 3.0f)
                {
                    if(activePipe->getGameArea() == PipeDestiny::COIN_ROOM)
                    {
                        currentArea = GameArea::COIN_ROOM;
                        camera.target = {screenWidth / 2.0f, screenHeight / 2.0f};
                        mario.setMarioAction(MarioAction::NORMAL);
                        mario.setPosition(activePipe->getDestination());
                    } else if(activePipe->getGameArea() == PipeDestiny::OVER_WORLD)
                    {
                        currentArea = GameArea::OVER_WORLD;
                        mario.setPosition(activePipe->getDestination());
                        mario.setMarioAction(MarioAction::EXITING_PIPE);
                        exitPipeTargetY = 400.0f;

                    }
                    pipeTimer = 0.0f;
                    activePipe = nullptr;
                }
            }
            // mario exiting for the pipe
            if(mario.getMarioAction() == MarioAction::EXITING_PIPE)
            {
                if(mario.getPosition().y <= exitPipeTargetY - mario.getHitbox().height)
                {
                    Vector2 position = mario.getPosition();

                    position.y = exitPipeTargetY - mario.getHitbox().height;

                    mario.setPosition(position);

                    mario.setMarioAction(
                        MarioAction::NORMAL
                    );
                }
            }

            for(Enemy& enemy : area->enemies) // check the collision of the enemies
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
                if(enemy.getHitbox().y >= screenHeight + 100)
                {
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
                            area->shells.push_back(Shell({enemy.getHitbox().x, enemy.getHitbox().y + 20}));
                        }
                    }
                    if(!mario.isInvincible()) { 
                        if(CheckCollisionRecs(mario.getLeftCheck(), enemy.getHitbox())
                                || CheckCollisionRecs(mario.getRightCheck(), enemy.getHitbox())
                                || CheckCollisionRecs(mario.getHeadCheck(), enemy.getHitbox())) // Check if Mario collides with the enemy
                        {
                            if(mario.getState() == MarioState::FIRE || mario.getState() == MarioState::BIG)
                            {
                                mario.setState(MarioState::SMALL);
                                mario.isHit();
                            }
                            else if(mario.getState() == MarioState::SMALL)
                            {
                                    mario.loselife();
                            }
                        }
                    }
                }
            

                for(Block& block : area->blocks) // Check for collision between blocks and enemies
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
                    if(CheckCollisionRecs(mario.getHeadCheck(), block.getHitbox()) 
                        && CheckCollisionRecs(enemy.getGroundCheck(), block.getHitbox()))
                    {
                        enemy.defeat();
                    }
                }
                for(Pipe& pipe : area->pipes)
                {
                    // enemy walking to the left
                    if(enemy.getVelocityX() < 0.0f &&
                    CheckCollisionRecs(
                        enemy.getLeftCheck(),
                        pipe.getHitbox()
                    ))
                    {
                        enemy.hitWallOnLeft(
                            pipe.getHitbox().x +
                            pipe.getHitbox().width
                        );
                    }

                    // enemy walking to the right
                    else if(enemy.getVelocityX() > 0.0f &&
                            CheckCollisionRecs(
                                enemy.getRightCheck(),
                                pipe.getHitbox()
                            ))
                    {
                        enemy.hitWallOnRight(
                            pipe.getHitbox().x
                        );
                    }

                    // inimigo em cima do cano
                    if(CheckCollisionRecs(
                        enemy.getGroundCheck(),
                        pipe.getEntranceCheck()
                    ))
                    {
                        enemy.LandOn(pipe.getHitbox().y);
                    }
                }

            }
            for(size_t i = 0; i < area->enemies.size(); i++) // Check collision between enemies and change their directions
                {
                    if(!area->enemies[i].isAlive()) continue;

                    for(size_t j = i + 1; j < area->enemies.size(); j++)
                    {
                        if(!area->enemies[j].isAlive()) continue;

                        Enemy& enemyA = area->enemies[i];
                        Enemy& enemyB = area->enemies[j];

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

            for(Shell& shell : area->shells)
            {
                if(!shell.isExisting()) continue;

                if(CheckCollisionRecs(mario.getHitbox(), shell.getHitBox()))
                {
                    // Mario with star destroy the shell
                    if(mario.getStarPower())
                    {
                        shell.destroy();
                    }

                    // ==============================
                    // STOPPED SHELL
                    // ==============================
                    else if(!shell.isMoving())
                    {
                        // Mario caiu por cima
                        if(CheckCollisionRecs(
                                mario.getGroundCheck(),
                                shell.getTopCheck())
                        && mario.isFalling())
                        {
                            mario.bounce();
                        }

                        // Mario touched on the side
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
                            else
                            {
                                shell.Kick(ShellDirection::LEFT);
                            }
                        }
                    }

                    // ==============================
                    // SHELL IN MOVIMENT
                    // ==============================
                    else
                    {
                        // Mario falls on top
                        if(CheckCollisionRecs(
                                mario.getGroundCheck(),
                                shell.getTopCheck())
                        && mario.isFalling())
                        {
                            shell.Stop();
                            mario.bounce();
                        }

                        // Mario get hit on the side
                        if(!mario.isInvincible()) {
                            if(shell.canDamageMario())
                            {
                                if(mario.getState() == MarioState::BIG ||
                                mario.getState() == MarioState::FIRE)
                                {
                                    mario.isHit();
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
                for(Enemy& enemy : area->enemies)
                {
                    if(CheckCollisionRecs(enemy.getHitbox(), shell.getHitBox()) && shell.isMoving())
                    {
                        enemy.defeat();
                    }
                }
                for(Block& block : area->blocks)
                {
                    if(CheckCollisionRecs(shell.getGroundCheck(), block.getHitbox()) && shell.isFalling())
                    {
                        shell.LandOn(block.getHitbox().y);
                    }
                    if(shell.isMoving()) {
                        if(CheckCollisionRecs(shell.getRightCheck(), block.getHitbox()) && shell.getVelocityX() > 0.0f)
                        {
                            shell.hitWallOnRight(block.getHitbox().x);
                        }
                        if(CheckCollisionRecs(shell.getLeftCheck(), block.getHitbox()) && shell.getVelocityX() < 0.0f)
                        {
                            shell.hitWallOnLeft(block.getHitbox().x + block.getHitbox().width);
                        }
                    }
                }
                for(Pipe& pipe : area->pipes) //check collision between the enemies with the pipes
                {
                    if(shell.isMoving()) {
                        if(CheckCollisionRecs(shell.getRightCheck(), pipe.getLeftCheck()) && shell.getVelocityX() > 0.0f)
                        {
                            shell.hitWallOnRight(pipe.getHitbox().x);
                        }
                        else if(CheckCollisionRecs(shell.getLeftCheck(), pipe.getRightCheck()) && shell.getVelocityX() < 0.0f)
                        {
                            shell.hitWallOnLeft(pipe.getHitbox().x + pipe.getHitbox().width);
                        }
                    }
                    if(CheckCollisionRecs(shell.getGroundCheck(), pipe.getEntranceCheck()) && shell.isFalling())
                    {
                        shell.LandOn(pipe.getHitbox().y);
                    }
                }
            }
            for(FireBall& fireball : area->fireballs) //check fireball collisions
            {
                if(!fireball.isActive()) continue;

                if(fireball.getHitbox().x > cameraArea.x + cameraArea.width 
                    || fireball.getHitbox().x < cameraArea.x) 
                    {
                        fireball.destroy();
                    }

                for(Enemy& enemy : area->enemies) 
                {
                    if(!enemy.isAlive()) continue;

                    if(CheckCollisionRecs(fireball.getHitbox(), enemy.getHitbox()))
                    {
                        fireball.destroy();
                        enemy.defeat();
                    }
                }
                for(Block& block : area->blocks) 
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
                for(Shell& shell : area->shells)
                {
                    if(CheckCollisionRecs(fireball.getHitbox(), shell.getHitBox()))
                    {
                        fireball.destroy();
                        shell.destroy();
                    }
                }
                for(Pipe& pipe : area->pipes)
                {
                    if(CheckCollisionRecs(fireball.getHitbox(), pipe.getHitbox()))
                    {
                        fireball.destroy();
                    }
                    if(CheckCollisionRecs(fireball.getGroundCheck(), pipe.getEntranceCheck()))
                    {
                        fireball.Bounce();
                    }
                }
            }
            for(Coin& coin : area->coins) 
            {
                if(!coin.isExisting()) continue;

                if(CheckCollisionRecs(mario.getHitbox(), coin.getHitbox()))
                {
                    coin.collectCoin();
                    mario.addCoin();
                }
            }
            // flag collision
            if(CheckCollisionRecs(mario.getRightCheck(), flag.getFlagPoleHitbox()))
            {
                flag.loweringTheFlag(deltaTime);
            }

            if(mario.getCoins() >= 100) {
                mario.gainlife();
                mario.resetCoins();
            }
            
            // ======
            //  Draw
            // ======

            BeginDrawing();
            if(currentArea == GameArea::OVER_WORLD)    
                ClearBackground(SKYBLUE);
            else if(currentArea == GameArea::COIN_ROOM) {
                Color DarkSky = {40, 36, 51, 255};
                ClearBackground(DarkSky);
            }
            BeginMode2D(camera);
                DrawCastle(7950.0f);
                flag.draw();
                mario.draw();
                for(Pipe& pipe : area->pipes) pipe.draw();
                for(PowerUp& powerUp : area->powerUps) powerUp.draw();
                for(Block& block : area->blocks) block.draw();
                for(Enemy& enemy : area->enemies) 
                {
                    if(enemy.isAlive()) enemy.draw();
                }
                for(Shell& shell : area->shells) shell.draw();
                for(FireBall& fireball : area->fireballs) fireball.draw();
                for(Coin& coin : area->coins) coin.draw();
                for(Decoration& decoration : area->decorations)
                {
                    if(decoration.type == DescorationType::PIPE_VERTICAL)
                    {
                        DrawRectangleRec(decoration.rect, GREEN);
                    }
                }
            EndMode2D();

            DrawText(TextFormat("Coins: %d", mario.getCoins()), 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("Lifes: %d", mario.getLifes()), 10, 40, 20, RAYWHITE);
            DrawText(TextFormat("Score: %d", score), 10, 70, 20, RAYWHITE);
            EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

void DrawCastle(float X)
{
    float groundY = 500.0f;
    Rectangle Base = {X, groundY - 210, 350, 210};
    Rectangle firstFloor = {X + 80, groundY - 320, 190, 110};
    Rectangle secondFloor = {X + 120, groundY - 400, 110, 80};
    Rectangle Gates;
    Rectangle Windows;
    DrawRectangleRec(secondFloor, DARKBROWN);
    DrawRectangleLinesEx(secondFloor, 5.0f, BEIGE);
    DrawRectangleRec({secondFloor.x + (secondFloor.width / 5), secondFloor.y + 20, secondFloor.width / 5, 40}, BLACK);
    DrawRectangleRec({secondFloor.x + 3*(secondFloor.width / 5), secondFloor.y + 20, secondFloor.width / 5, 40}, BLACK);
    DrawRectangleRec(firstFloor, DARKBROWN);
    DrawRectangleLinesEx(firstFloor, 5.0f, BEIGE);
    DrawRectangleRec({firstFloor.x + 2*(firstFloor.width / 5), firstFloor.y + (firstFloor.height / 4), Base.width / 9, Base.height / 3}, BLACK);
    DrawRectangleRec(Base, DARKBROWN);
    for(int i = 1; i <= 3; i ++) 
    {
        for(int k = 2; k <= 6; k++)
        {
            Gates = {Base.x + k * (Base.width / 9), Base.y + 2 * (Base.height / 3), Base.width / 9, Base.height / 3};
            Windows = {Base.x + k* (Base.width / 9), Base.y + (Base.height / 3) - 40, Base.width / 9, Base.height / 3};
            if(k % 2 == 0)    
                DrawRectangleRec(Gates, BLACK);
            else
                DrawRectangleRec(Windows, BLACK);
        }
    }
    DrawRectangleLinesEx(Base, 5.0f, BEIGE);
}

