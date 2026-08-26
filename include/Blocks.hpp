#pragma once
#include <raylib.h>

enum BlockType { SOLID, BRICK, QUESTION, FLOOR};
enum class BlockContent { NONE, COIN, POWERUP, STAR };
class Block {
public:
    Block(Rectangle rect, BlockType blockType, BlockContent Content = BlockContent::NONE);
    void draw();
    bool isDestroyed() const;
    void BrickDestroyed();
    BlockContent activate();
    Rectangle getHitbox() const;
    BlockType getType() const;
    BlockContent getContent() const;
    bool isBlockUsed() const;
private:
    Rectangle hitbox;
    BlockType type;
    BlockContent content;
    bool isUsed;
    bool isBroken;
};