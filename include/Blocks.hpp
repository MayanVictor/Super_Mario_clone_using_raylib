#pragma once
#include <raylib.h>

enum BlockType { SOLID, BRICK, QUESTION};
enum class BlockContent {NONE, COIN, MUSHROOM, FIRE_FLOWER};
class Block {
public:
    Block(Rectangle rect, BlockType blockType, BlockContent Content = BlockContent::NONE);
    void draw();
    Rectangle getHitbox() const;
    BlockType getType() const;
    BlockContent getContent() const;
private:
    Rectangle hitbox;
    BlockType type;
    BlockContent content;
    bool isUsed;
    bool isBroken;
};