#include"Blocks.hpp"
#include<raylib.h>

Block::Block(Rectangle rect, BlockType blockType, BlockContent Content) 
        : hitbox(rect), type(blockType), content(Content), isUsed(false), isBroken(false) {}
void Block::draw() 
{
    Color blockColor = GRAY;

    if(isBroken) 
    {
        return; // Don't draw broken bricks
    }
    switch (type) 
    {
        case SOLID:
            blockColor = GRAY;
            break;
        case BRICK:
            blockColor = BROWN;
            break;
        case QUESTION:
            blockColor = isUsed ? DARKGRAY : YELLOW;
            break;
        case FLOOR:
            blockColor = {176, 51, 20, 255}; // an red-orange color for the floor
            break;

    }
    DrawRectangleRec(hitbox, blockColor);
}
bool Block::isDestroyed() const
{
    return isBroken;
}
void Block::BrickDestroyed() 
{
    if(type == BRICK && !isBroken) 
    {
        isBroken = true;
        isUsed = true;
    }
}
BlockContent Block::activate() 
{
    if(type != QUESTION || isUsed)
    {
        return BlockContent::NONE;
    }

    isUsed = true;

    BlockContent released = content;
    content = BlockContent::NONE;

    return released;
}
Rectangle Block::getHitbox() const 
{
    return hitbox;
}
BlockContent Block::getContent() const 
{
    return content;
}
BlockType Block::getType() const 
{
    return type;
}
bool Block::isBlockUsed() const 
{
    return isUsed;
}