#include"Blocks.hpp"
#include<raylib.h>

Block::Block(Rectangle rect, BlockType blockType, BlockContent Content) 
        : hitbox(rect), type(blockType), content(Content), isUsed(false), isBroken(false) {}
void Block::draw() 
{
    Color blockColor = GRAY;
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
    }
    DrawRectangleRec(hitbox, blockColor);
}
BlockContent Block::activate() 
{
    if(type != QUESTION || isUsed)
    {
        return BlockContent::NONE;
    }

    isUsed = true;

    if(content == BlockContent::COIN) 
    {
        return BlockContent::COIN;
    } 
    else if(content == BlockContent::MUSHROOM) 
    {
        return BlockContent::MUSHROOM;
    } 
    else if(content == BlockContent::FIRE_FLOWER) 
    {
        return BlockContent::FIRE_FLOWER;
    } 
    else 
    {
        return BlockContent::NONE;
    } 

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