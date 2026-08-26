#pragma once
#include <raylib.h>

enum class PipeType {NORMAL,ENTRANCE,EXIT};
enum class PipeDirection { UP, LEFT };
enum class PipeDestiny {OVER_WORLD, NONE ,COIN_ROOM};

class Pipe 
{
    public:
        Pipe(Rectangle rect, PipeType type, Vector2 dest = {0, 0}, 
            PipeDestiny area = PipeDestiny::NONE, 
            PipeDirection dir = PipeDirection::UP);

        void draw();

        Rectangle getHitbox();
        Rectangle getEntranceCheck();
        Rectangle getRightCheck();
        Rectangle getLeftCheck();

        PipeType getType() const;
        PipeDestiny getGameArea() const;
        PipeDirection getDirection() const;

        Vector2 getDestination();
    private:
        Rectangle hitbox;
        Vector2 arrival;

        PipeDestiny destiny;
        PipeDirection direction;
        PipeType type;
};